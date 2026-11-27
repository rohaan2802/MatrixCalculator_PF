# Architecture and design

MatrixCalculator_PF is intentionally small enough to inspect in one sitting, while its evidence and presentation tools make the behavior reproducible.

## Layers

| Layer | Responsibility | Primary files |
| --- | --- | --- |
| Native application | 19-option console workflow and numerical operations | `src/matrix_calculator.cpp` |
| Verification | Known inputs, regression assertions and replayed stdout | `tests/`, `data/`, `examples/` |
| Browser playground | Live local calculations, JSON export, themes and gallery controls | `index.html`, `assets/js/`, `assets/css/` |
| Presentation | Shared README/site content, figures and provenance reports | `tools/build_portfolio.py`, `docs/reports/` |

## Data flow

```text
data/demo_cases.json
        │
        ▼
native executable ──► examples/transcripts/ ──► docs/reports/native_results.json
        │                                      │
        └──────────────► regression checks     └──► 24 README / Pages figures

browser inputs ──► assets/js/matrix.js ──► local result + JSON download
```

The C++ and browser calculators share row equilibration, partial pivoting, finite-value checks and bounded 1–10 dimensions, but remain separate entry points. The replay data is the contract between the executable and the evidence gallery.

Figures are generated from captured executable output. The browser playground is presented as a live local calculation, so the saved evidence and interactive behavior remain easy to distinguish.

## Reproduce

```text
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build
ctest --test-dir build --output-on-failure
python tools/collect_results.py
python tools/build_portfolio.py
```
