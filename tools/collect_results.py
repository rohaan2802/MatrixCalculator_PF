"""Run recorded input cases through the native console and save real output."""
import json
import hashlib
import os
from pathlib import Path
import subprocess

ROOT = Path(__file__).resolve().parents[1]

def executable(name):
    suffix = ".exe" if os.name == "nt" else ""
    for path in (ROOT / "build" / (name + suffix), ROOT / "build/Release" / (name + suffix)):
        if path.is_file():
            return path
    raise FileNotFoundError("Build the CMake targets before collecting results.")

def run():
    cases = json.loads((ROOT / "data/demo_cases.json").read_text())
    records = []
    for case in cases:
        folder = ROOT / ".test-output" / case["id"]
        folder.mkdir(parents=True, exist_ok=True)
        if case["operation"] is None:
            tokens = []
            command = [str(executable("matrix_regression"))]
        else:
            matrix = case["matrix"]
            tokens = ["1", str(len(matrix)), "1"]
            tokens += [str(value) for row in matrix for value in row]
            tokens += [str(case["operation"]), *case["extra"]]
            if case["id"] == "saved_history":
                tokens += ["Y", "17", "Y", "18", "Y", "19"]
            tokens += ["0"]
            command = [str(executable("matrix_calculator"))]
        completed = subprocess.run(
            command, input="\n".join(tokens) + "\n", text=True,
            encoding="utf-8", errors="replace", capture_output=True, cwd=folder, timeout=10
        )
        if completed.returncode != 0:
            raise RuntimeError(case["id"] + ": " + completed.stderr)
        transcript = completed.stdout
        for expected in case["contains"]:
            if expected not in transcript:
                raise AssertionError(case["id"] + " missing " + expected + "\n" + transcript[-2200:])
        output = ROOT / "examples/transcripts" / (case["id"] + ".txt")
        output.parent.mkdir(parents=True, exist_ok=True)
        output.write_text(transcript, encoding="utf-8")
        focus = transcript
        marker = transcript.rfind("  Enter your choice: ", 0, transcript.find("  What next?"))
        if marker >= 0:
            focus = transcript[marker + len("  Enter your choice: "):]
        if "  What next?" in focus:
            focus = focus.split("  ---------------------------------------------------------------------\n  What next?")[0]
        focus = focus.strip()
        if case["id"] == "saved_history":
            focus = (folder / "result.txt").read_text(encoding="utf-8")
            (ROOT / "examples/saved_result.txt").write_text(focus, encoding="utf-8")
            if "Result operation : Transpose" not in focus:
                raise AssertionError("Saved calculation provenance was lost.")
        records.append({
            **case, "passed": True,
            "transcript": output.relative_to(ROOT).as_posix(),
            "focus": focus,
        })
    for label, tokens in (
        ("matrix input EOF", "1\n2\n1\n4\n"),
        ("vector input EOF", "1\n1\n1\n2\n7\n1\n"),
        ("scalar input EOF", "1\n1\n1\n2\n13\n1\n"),
        ("overflowing size", "1\n4294967297\n0\n0\n"),
    ):
        completed = subprocess.run(
            [str(executable("matrix_calculator"))], input=tokens, text=True,
            encoding="utf-8", capture_output=True, cwd=ROOT / ".test-output", timeout=5
        )
        if completed.returncode:
            raise AssertionError(label + " failed")
    source = (ROOT / "src/matrix_calculator.cpp").read_bytes().replace(b"\r\n", b"\n")
    report = {"source": "src/matrix_calculator.cpp", "source_sha256": hashlib.sha256(source).hexdigest(), "mode": "native executable replay", "cases": len(records), "passed": len(records),
              "input_flow_checks": 4, "records": records}
    (ROOT / "docs/reports/native_results.json").write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"Recorded {len(records)} passing cases and 4 input-flow checks.")

if __name__ == "__main__":
    run()
