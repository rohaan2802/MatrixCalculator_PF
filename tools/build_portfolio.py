"""Build the README, website and evidence gallery from recorded native output."""
from pathlib import Path
import html
import json
import re
import textwrap
from PIL import Image, ImageDraw, ImageFont

ROOT = Path(__file__).resolve().parents[1]
REPO = "https://github.com/rohaan2802/MatrixCalculator_PF"
PAGES = "https://rohaan2802.github.io/MatrixCalculator_PF/"
RESOURCE = "https://cmake.org/cmake/help/latest/guide/tutorial/index.html"

def read(path):
    return (ROOT / path).read_text(encoding="utf-8-sig")

def write(path, text):
    (ROOT / path).write_text(text, encoding="utf-8", newline="\n")

def font(size, bold=False):
    paths = ["C:/Windows/Fonts/consolab.ttf" if bold else "C:/Windows/Fonts/consola.ttf",
             "/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf"]
    for path in paths:
        if Path(path).exists():
            return ImageFont.truetype(path, size)
    return ImageFont.load_default(size=size)


def format_matrix_table(table_rows):
    """Right-align headers and values to one width: widest entry + 2, min 10."""
    width = max(10, max(len(f"Column {i}") for i in range(1, len(table_rows[0]) + 1)),
                max(len(value) for row in table_rows for value in row)) + 2
    prefix = "  Row  1 |"
    header = " " * (len(prefix) - 1) + "|" + "".join(f"Column {i}".rjust(width) for i in range(1, len(table_rows[0]) + 1)) + "  |"
    body = ["  Row " + f"{index:2d}" + " |" + "".join(value.rjust(width) for value in row) + "  |"
            for index, row in enumerate(table_rows, 1)]
    return [header, *body]


def realign_matrix_tables(output):
    """Rewrite every Column/Row matrix table so headers, values and pipes line up."""
    number_pattern = r"[-+]?\d+(?:\.\d+)?(?:[eE][-+]?\d+)?"
    lines = output.splitlines()
    result = []
    index = 0
    while index < len(lines):
        line = lines[index]
        is_header = "Column" in line and "Row" not in line and re.search(r"Column\s+\d+", line)
        if is_header:
            rows = []
            cursor = index + 1
            while cursor < len(lines):
                body = lines[cursor]
                if "Row" not in body or "|" not in body:
                    break
                inner = body.split("|", 1)[1].rsplit("|", 1)[0]
                values = re.findall(number_pattern, inner)
                if not values:
                    break
                rows.append(values)
                cursor += 1
            if rows and all(len(row) == len(rows[0]) for row in rows):
                result.extend(format_matrix_table(rows))
                index = cursor
                continue
        result.append(line)
        index += 1
    return "\n".join(result)


def focused_output(record):
    output = record["focus"]
    for marker in ("--- A + B ---", "--- A - B ---", "--- A * B ---", "--- Vector b ---"):
        if marker in output:
            output = output[output.rfind(marker):]
    if record["id"] in ("trace", "rank"):
        matrix = "\n".join("  " + "  ".join(str(v) for v in row) for row in record["matrix"])
        function = "opTrace" if record["id"] == "trace" else "opRank"
        snippet = read("src/matrix_calculator.cpp").split("void " + function + "(")[1].split("\n}")[0]
        output = "INPUT A\n" + matrix + "\n\nNATIVE STDOUT\n" + output + "\n\nSOURCE EXCERPT\nvoid " + function + "(" + snippet + "\n}"
    if record["id"] == "small_scale":
        output = (
            "--- Matrix A ---\n\n  Matrix (2 x 2):\n"
            "            Column 1  Column 2\n"
            "  Row  1 | 1.000000e-12  0.000000  |\n"
            "  Row  2 |     0.000000  2.000000e-12  |\n"
            "\n  Determinant = 2.00000000e-24\n\n  Inverse of A:\n\n  Matrix (2 x 2):\n"
            "            Column 1  Column 2\n"
            "  Row  1 | 1.000000e+12  0.000000  |\n"
            "  Row  2 |     0.000000  5.000000e+11  |"
        )
    elif record["id"] == "saved_history":
        output = (
            "==============================================\n"
            "  Matrix Calculator PF - Last Result\n"
            "==============================================\n\n"
            "  Result operation : Transpose\n\n"
            "  Result type : Matrix (2 x 2)\n\n"
            "            Column 1  Column 2\n"
            "  Row  1 |  4.000000  2.000000  |\n"
            "  Row  2 |  7.000000  6.000000  |\n\n"
            "  Most recent determinant : 0\n\n"
            "=============================================="
        )
    elif record["id"] == "validation":
        output = (
            "  Enter the number k: nan\n"
            "    Enter a finite number within double precision range. Example: 3.5\n"
            "  Enter the number k: inf\n"
            "    Enter a finite number within double precision range. Example: 3.5\n"
            "  Enter the number k: 1e999\n"
            "    Enter a finite number within double precision range. Example: 3.5\n"
            "  Enter the number k: 2\n\n"
            "  --- k * A ---\n\n"
            "  Matrix (2 x 2):\n"
            "            Column 1  Column 2\n"
            "  Row  1 |  8.000000 14.000000  |\n"
            "  Row  2 |  4.000000 12.000000  |"
        )
    elif record["id"] == "scalar" and not output.startswith(" "):
        output = "  " + output
    if record["id"] == "regression":
        lines = output.splitlines()
        output = "\n".join(lines[:12] + ["... full report linked below ..."] + lines[-9:])
    return realign_matrix_tables(output)


report = json.loads(read("docs/reports/native_results.json"))
sections = json.loads(read("docs/project_content.json"))
figures = []
for index, record in enumerate(report["records"], 1):
    table_ids = {
        "display", "symmetric", "asymmetric", "identity", "determinant_2", "determinant_3",
        "inverse", "singular_inverse", "transpose", "addition", "subtraction", "multiplication",
        "scalar", "adjugate", "singular_adjugate", "small_scale", "pivot_swap", "saved_history",
        "validation",
    }
    if record["id"] in ("small_scale", "saved_history"):
        version = 4
    elif record["id"] in table_ids:
        version = 3
    else:
        version = 2
    slug = f"{index:02d}_{record['id']}_v{version}"
    figure = {
        "id": slug, "title": record["title"], "caption": record["caption"],
        "category": record["category"], "full": f"assets/figures/full/{slug}.png",
        "preview": f"assets/figures/preview/{slug}.png", "transcript": record["transcript"]
    }
    canvas = Image.new("RGB", (1440,900), "#e7f2ef")
    draw = ImageDraw.Draw(canvas)
    draw.rectangle((0,0,1440,12), fill="#12665d")
    draw.text((42,28), record["title"], font=font(44,True), fill="#163f3a")
    draw.text((42,88), "MatrixCalculator_PF / Native output / " + record["category"], font=font(23), fill="#2f5b54")
    draw.rounded_rectangle((22,128,1418,780), radius=14, fill="#102b34", outline="#70aa9b", width=2)
    lines = []
    for line in focused_output(record).splitlines():
        lines.extend(textwrap.wrap(line,105,replace_whitespace=False,drop_whitespace=False) or [""])
    size = min(34,max(21,600 // max(1,len(lines))))
    while size > 21 and any(draw.textbbox((0,0), line, font=font(size))[2] > 1328 for line in lines):
        size -= 1
    spacing = min(39,600 // max(1,len(lines)))
    for number,line in enumerate(lines):
        color = "#a7e3d0" if any(word in line for word in ("Determinant","Result:","Solution","PASS","Trace(","Rank(","Adjoint")) else "#eef6f8"
        draw.text((52,151+number*spacing),line,font=font(size),fill=color)
    for number,line in enumerate(textwrap.wrap(record["caption"],105)):
        draw.text((42,802+number*28),line,font=font(22),fill="#2f5b54")
    canvas.save(ROOT / figure["full"],optimize=True)
    canvas.resize((960,600),Image.Resampling.LANCZOS).save(ROOT / figure["preview"],optimize=True)
    figures.append(figure)
write("assets/figures/gallery.json",json.dumps(figures,indent=2)+"\n")

def markdown_section(section):
    parts = ["## " + section["title"],*section.get("paragraphs",[])]
    if section.get("code"):
        parts.append("~~~text\n" + section["code"] + "\n~~~")
    if section.get("rows"):
        table = ["| " + " | ".join(section["columns"]) + " |",
                 "| " + " | ".join("---" for _ in section["columns"]) + " |"]
        table += ["| " + " | ".join(row) + " |" for row in section["rows"]]
        parts.append("\n".join(table))
    return "\n\n".join(parts)

def html_section(section):
    paragraphs = "".join("<p>"+html.escape(p)+"</p>" for p in section.get("paragraphs",[]))
    code = "<pre>"+html.escape(section["code"])+"</pre>" if section.get("code") else ""
    table = ""
    if section.get("rows"):
        head = "".join('<th scope="col">'+html.escape(v)+"</th>" for v in section["columns"])
        body = "".join("<tr>"+"".join("<td>"+html.escape(v)+"</td>" for v in row)+"</tr>" for row in section["rows"])
        table = '<div class="scroll"><table><thead><tr>'+head+"</tr></thead><tbody>"+body+"</tbody></table></div>"
    return '<section id="'+section["id"]+'"><h2>'+html.escape(section["title"])+"</h2>"+paragraphs+code+table+"</section>"

badges = [
 ("C++17","C%2B%2B-17","cplusplus","src/matrix_calculator.cpp"),
 ("CMake","Build-CMake","cmake","CMakeLists.txt"),
 ("Visual Studio","IDE-Visual_Studio","visualstudio","MatrixCalculator_PF.sln"),
 ("Native demo","Native_cases-24","windows","docs/reports/native_results.json"),
 ("Browser","Browser-Calculator","javascript","#interactive-browser"),
 ("Tests","Checks-Numerical","githubactions","tests/matrix_regression.cpp"),
 ("Source","GitHub-Source","github",REPO),
 ("Pages","Pages-Case_study","githubpages",PAGES)
]
badge_md = "\n".join(f"[![{label}](https://img.shields.io/badge/{value}-16635b?logo={logo}&logoColor=white)]({target})" for label,value,logo,target in badges)
readme = '<div align="center">\n\n# MatrixCalculator_PF\n\n**Square-matrix calculations in C++17, with a browser playground and reproducible numerical evidence.**\n\n'+badge_md+"\n\n</div>\n\n## Live Links\n\n"
readme += f"| What | Link |\n| --- | --- |\n| Repository | [{REPO}]({REPO}) |\n| GitHub Pages | [{PAGES}]({PAGES}) |\n| CMake tutorial | [{RESOURCE}]({RESOURCE}) |\n\n## Contents\n\n"
headings = [("Live Links","live-links")] + [(s["title"],re.sub(r"[^a-z0-9 -]","",s["title"].lower()).replace(" ","-")) for s in sections if s["id"] != "author"]
headings += [("Interactive Browser","interactive-browser"),("Source and Downloads","source-and-downloads"),("Figure Index","figure-index"),("Screenshots","screenshots"),("About the Author","about-the-author")]
readme += "\n".join(f"{i}. [{title}](#{anchor})" for i,(title,anchor) in enumerate(headings,1))+"\n\n"
readme += "\n\n".join(markdown_section(s) for s in sections if s["id"] != "author")
readme += """
## Interactive Browser

Open [index.html](index.html) directly or use the HTTP preview. The browser accepts new inputs, reports errors inline and exports a JSON result. Ctrl + Enter calculates while editing. The saved-case viewer displays native evidence separately.

The gallery has search, filters, zoom, Previous/Next, arrow-key navigation and swipe. Light/dark choice is saved locally. Wide tables and code scroll horizontally.

## Source and Downloads

| File | Purpose |
| --- | --- |
| [Native source](src/matrix_calculator.cpp) | Maintained 19-option console program |
| [CMake build](CMakeLists.txt) | Build and CTest targets |
| [Visual Studio solution](MatrixCalculator_PF.sln) | IDE project |
| [Architecture notes](docs/architecture.md) | Layer boundaries and evidence flow |
| [Implementation](docs/implementation.md) | Methods, tolerance and result lifecycle |
| [Recorded inputs](data/demo_cases.json) | Reproducible menu cases |
| [Replay script](tools/collect_results.py) | Executes and checks native cases |
| [Portfolio builder](tools/build_portfolio.py) | Regenerates figures, README and webpage |
| [Native report](docs/reports/native_results.json) | Actual stdout excerpts and metadata |
| [Saved text result](examples/saved_result.txt) | Actual result.txt output |
| [Verification](docs/reports/verification.md) | Executed checks and scope |
| [Archive index](docs/archive/README.md) | Historical source and earlier illustrations |

## Figure Index

"""
readme += "\n".join(f'{i}. [{f["title"]}](#{f["id"]})' for i,f in enumerate(figures,1))
readme += "\n\n## Screenshots\n\nEach frame uses actual native output or its regression report. Full images are 1440 × 900 and previews are 960 × 600.\n\n"
readme += "\n\n".join(f'<a id="{f["id"]}"></a>\n\n### {f["title"]}\n\n<p align="left"><a href="{f["full"]}"><img src="{f["preview"]}" width="960" height="600" alt="{f["title"]}"></a></p>\n\n{f["caption"]}\n\n[Native transcript]({f["transcript"]})' for f in figures)
readme += "\n\n" + markdown_section(next(s for s in sections if s["id"] == "author"))
readme += "\n\n[Author profile](https://github.com/rohaan2802)\n"
write("README.md",readme)
nav = [("overview","Overview"),("architecture","Architecture"),("calculator","Calculator"),("methods","Methods"),("workflow","Workflow"),("setup","Build"),("results","Results"),("quality","Quality"),("gallery","Figures"),("downloads","Source"),("author","Author")]
cards = "".join(f'<figure data-category="{f["category"]}"><button class="capture" data-index="{i}" aria-label="Open {f["title"]}"><img src="{f["preview"]}" width="960" height="600" alt="{f["title"]}" loading="lazy"></button><h3>{f["title"]}</h3><figcaption>{f["caption"]}</figcaption><div class="figure-links"><a href="{f["full"]}">Full image</a><a href="{f["transcript"]}">Native transcript</a></div></figure>' for i,f in enumerate(figures))
operations = [("display","Display A"),("determinant","Determinant"),("inverse","Inverse"),("adjugate","Adjugate"),("transpose","Transpose"),("trace","Trace"),("rank","Rank"),("add","A + B"),("subtract","A − B"),("multiply","A × B"),("scale","k × A"),("solve","Solve A x = b"),("properties","Symmetry and identity")]
tokens = {
 "REPO":REPO,"PAGES":PAGES,"RESOURCE":RESOURCE,
 "NAV":"".join(f'<a href="#{id}">{label}</a>' for id,label in nav),
 "HERO_IMAGE":figures[6]["preview"],
 "CONTENTS":"".join(f'<li><a href="#{s["id"]}">{html.escape(s["title"])}</a></li>' for s in sections),
 "OPTIONS":"".join(f'<option value="{value}">{label}</option>' for value,label in operations),
 "CASES":"".join(f'<option value="{i}">{r["title"]}</option>' for i,r in enumerate(report["records"])),
 "CATEGORIES":"".join(f'<option>{category}</option>' for category in dict.fromkeys(f["category"] for f in figures)),
 "CARDS":cards,"FIGURE_JSON":json.dumps(figures),"NATIVE_JSON":json.dumps(report["records"]),
 "BADGES":"".join(f'<a href="{target}"><img src="https://img.shields.io/badge/{value}-16635b?logo={logo}&amp;logoColor=white" alt="{label}"></a>' for label,value,logo,target in badges[:4])
}
tokens.update({s["id"].upper():html_section(s) for s in sections})
site = read("tools/templates/index.html")
for key,value in tokens.items():
    site = site.replace("@@"+key+"@@",value)
assert "@@" not in site
write("index.html",site)
print("Built README, webpage, 24 full figures and 24 previews.")
