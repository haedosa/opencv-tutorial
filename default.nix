{ stdenv, opencv4, cmake, ... }:

stdenv.mkDerivation {
  name = "opencv-tutorial";

  src = ./.;

  buildInputs = let
    opencvGtk = opencv4.override (old: { enableGtk2 = true; });
  in
    [
      opencvGtk
      cmake
    ];
}
