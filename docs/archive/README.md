# Project audit and archive

The repository was cloned from rohaan2802/MatrixCalculator_PF at commit 0d90c474b642d2039a0a3874815717d9357dbe38. The existing master branch and Git history are preserved. The GitHub repository name is MatrixCalculator_PF.

There was one C++ implementation, one Visual Studio solution, two Windows build/launch scripts, one saved text report, and 30 generated gallery panels. No ZIP archives, external dataset, or competing source builds were present.

The maintained calculator is now src/matrix_calculator.cpp. The Visual Studio solution and project are named MatrixCalculator_PF. CMakeLists.txt adds a C++17 build and numerical regression target. run.bat and build.ps1 use that maintained build. The old separate MSVC builder was replaced by these shared build entry points.

The unchanged source is preserved in console_v1.cpp. The former 30 panels and their generator are in legacy_gallery. They contain authored console-style examples and are identified as historical illustrations. The current 24 figures use recorded native executable output. The original saved text report is in examples/original_saved_result.txt.

The source fixes cover finite input validation, integer range checking before narrowing, EOF handling during matrix/vector/scalar entry, row-equilibrated elimination, larger singular adjugates, result provenance and safer file replacement. Numerical behavior and remaining precision limits are documented in docs/implementation.md.

Build output, temporary execution folders and caches are excluded from Git. The complete source, reproducible build, example inputs, actual transcripts and generated presentation assets are maintained in the repository. All public assets are small enough for the repository.
