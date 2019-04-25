{Pkgs ? import ./Support/Pkgs.nix {}}:
{
    Truec = Pkgs.callPackage ./Support/Truec.nix {};
}
