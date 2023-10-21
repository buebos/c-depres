# C Dependency Resolver
## Resolve dependency tree of an entry file and compile all into an executable with a simple command

### Constraints
- Header files must be the same name as their .c counterpart and coexist as siblings in the same directory.
- Include tags must be declared relative to the .c file they are in.
- When depres finds a circular dependency it will not prompt you. Instead it will skip the second include tag to that dependency and just ignore it. When depres tries to compile it will lead to a reference error on gcc of the files you needed.
