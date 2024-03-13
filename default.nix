{ stdenv, opencv-custom, cmake, ... }:

stdenv.mkDerivation {
  name = "opencv-tutorial";

  src = ./.;

  buildInputs = let
    opencvGtk = opencv-custom.override (old: { enableGtk2 = true; });
  in
    [
      opencvGtk
      cmake
    ];

  cmakeFlags = [
  ];
}
