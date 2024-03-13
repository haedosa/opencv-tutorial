{
  inputs = {
    haedosa.url = "github:haedosa/flakes";
    nixpkgs.follows = "haedosa/nixpkgs";
  };

  outputs = { self, nixpkgs, ... }@inputs:
  let
    system = "x86_64-linux";
    pkgs = import nixpkgs { inherit system; overlays = [self.overlay]; };
  in rec {
    devShells.${system} = {
      default = pkgs.mkShell {
        buildInputs = [
          pkgs.pkg-config
          pkgs.opencv4
          pkgs.cmake
          pkgs.clang-tools_12
        ];
      };
    };

    packages.${system} = {
      default = pkgs.opencv-tutorial;
    };

    overlay = final: prev:
    {
      opencv-tutorial = final.callPackage ./. {};
    };

  };
}
