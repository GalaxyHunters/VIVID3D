If you would like to contribute to VIVID3D, here is the basic guideline:

- Create a fork of VIVID3D on your account
- When you want to fix a bug or add a new feature, create a branch on your own fork following these [instructions](https://docs.github.com/en/pull-requests/collaborating-with-pull-requests/proposing-changes-to-your-work-with-pull-requests/creating-a-pull-request-from-a-fork).
- The source code for vivid is located in `/src/`. Any changes or additions to the API including types, function signatures, class definitions, etc. also require updating the relevant wrapping code in `/src/wrapping/` and the documentation in `/docs/source/`.
- Push the changes to your fork of the repository and open a pull request to the `VIVID3D/master` branch.
- After Code Review and compilation checks we will merge your PR.
