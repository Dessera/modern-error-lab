{
  stdenv,
  lib,
  cmake,
  ninja,
}:
stdenv.mkDerivation {
  pname = "modern-error-lab";
  version = "0.1.0";

  src = lib.cleanSource ./.;

  nativeBuildInputs = [
    cmake
    ninja
  ];

  cmakeFlags = [
    "-DCMAKE_BUILD_TYPE=Release"
  ];
}
