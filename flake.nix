{
  description = "Error handling laboratory for modern cpp.";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixpkgs-unstable";
    systems.url = "github:nix-systems/default";

    flake-parts.url = "github:hercules-ci/flake-parts";
  };

  outputs =
    { flake-parts, systems, ... }@inputs:
    flake-parts.lib.mkFlake { inherit inputs; } {
      systems = import systems;

      perSystem =
        { self', pkgs, ... }:
        let
          stdenv = pkgs.gcc14Stdenv;

          clang-tools = pkgs.callPackage ./nix/clang-tools.nix {
            inherit stdenv;
          };
        in
        {
          packages.default = pkgs.callPackage ./default.nix { inherit stdenv; };

          devShells.default =
            pkgs.mkShell.override
              {
                inherit stdenv;
              }
              {
                inputsFrom = [ self'.packages.default ];
                hardeningDisable = [ "fortify" ];

                packages =
                  (with pkgs; [
                    nixd
                    nixfmt-rfc-style
                    cmake-format
                    doxygen
                  ])
                  ++ [
                    clang-tools
                  ];
              };
        };
    };
}
