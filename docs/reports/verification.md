# Verification

Verified locally on 23 November 2026. These are saved checks of the maintained source, not a claim that every platform or numerical input has been tested.

| Check | Result |
| --- | --- |
| Fresh native build | Passed with GCC 15.2.0, CMake and Ninja on Windows, using build.ps1 |
| CTest | 1/1 regression executable passed; 33 assertions |
| Native menu replay | 24/24 recorded cases passed |
| Additional input flow | 4/4 checks passed: truncated matrix, vector and scalar input, and overflowing size token |
| JavaScript arithmetic | 5 Node test groups passed |
| Responsive browser | Chrome at 1440, 1024, 390 and 320 pixels, in light and dark themes |
| Browser controls | Calculator, invalid input, JSON download, menu, theme, active navigation, search, filters, zoom, previous/next, keyboard, simulated swipe and Back to Top passed |
| Gallery parity | 24 matching figures, captions and order in README/site; 24 full images at 1440 × 900 and 24 previews at 960 × 600 |
| Local assets | Page anchors, local files and downloads checked; direct file preview calculated successfully |
| Source provenance | Recorded native source SHA-256 matches the maintained source after line-ending normalization |
| Git | Original master branch and history retained; no commits, staging or pushes performed |

## Evidence and reproduction

- Native replay and source hash: [native_results.json](native_results.json)
- Browser check results: [browser_checks.json](browser_checks.json)
- Complete stdout: [examples/transcripts](../../examples/transcripts)
- Numerical behavior and limitations: [implementation.md](../implementation.md)
- Build and replay commands: [README](../../README.md#setup-and-build)

Native source SHA-256: `d2432821f1fb847dc388a0330ca0dffd02035afd2b611071a0e91ef40c94fa13`.

Build products, browser viewport captures and scratch execution directories were removed after verification. The build scripts and IDE project recreate the executable locally. The 24 public figure pairs and recorded transcripts remain in the repository.

## Not verified here

Visual Studio compilation, Linux/macOS terminal behavior, other browser engines, physical touch devices and a deployed GitHub Pages site were not tested. Swipe was exercised with simulated touch events. The Visual Studio project paths were checked, and the owner retains control of publication. Remote Repository/Pages links identify the unchanged project URLs; this local pass does not publish their contents.

Floating-point limitations remain: nearly singular matrices, tolerance decisions and values outside the representable range require care. See the implementation notes rather than interpreting passing fixtures as proof of exact arithmetic.
