{stdenv, llvm}:
stdenv.mkDerivation {
    name = "Truec";
    src = ../Source;
    buildInputs = [llvm];
    phases = [
        "unpackPhase"
        "buildPhase"
        "installPhase"
        "fixupPhase"
    ];
    buildPhase = ''
        CXXFLAGS=(
            -std=c++17 -Wall -Wextra -Werror -Wpedantic
            -O2
            -lLLVM
        )

        g++ "''${CXXFLAGS[@]}" *.cpp
    '';
    installPhase = ''
        mkdir -p "$out/bin"
        mv a.out "$out/bin/Truec"
    '';
}
