{ stdenv, opencv-custom, cmake, cudatoolkit, ... }:

stdenv.mkDerivation {
  name = "opencv-tutorial";

  src = ./.;

  buildInputs = let
    my-opencv = opencv-custom.override
      (old: { enableGtk2 = true;
              enableCuda = true;
            });
  in
    [
      my-opencv
      cudatoolkit
      cmake
    ];

  cmakeFlags = [
  ];
}
