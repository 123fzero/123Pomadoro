# 123Pomadoro

## Versioning
- Current app version is tracked in `VERSION` using semantic versioning.
- Every commit must bump the version in `VERSION` using semver.
- Every commit must keep `application.fam` `fap_version` aligned to the same major/minor version.
- Every version commit must also create a matching git tag in the form `vX.Y.Z`.

## Release Checklist
- Bump `VERSION`.
- Update `application.fam` `fap_version`.
- Commit the changes.
- Create git tag `vX.Y.Z` on that commit.
