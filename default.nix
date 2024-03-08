{ stdenv, opencv4, pkg-config, cmake, ... }:

stdenv.mkDerivation {
  name = "opencv-tutorial";

  src = ./.;

  buildInputs = let
    opencvGtk = opencv4.override (old: { enableGtk2 = true; });
  in
    [
      opencvGtk
      pkg-config
      cmake
    ];
}
