# Cow

## WIP
This is a work in progress. Apis can change at any time.

Cow is a very simple parser generator.

## Name
I honestly don't know. I just thought that there is bison from gnu so my simple (worse) parser generator could be a cow.

## Grammar

```
digit = range '0' '9'
char = {
    range 'a' 'b'
    range 'A' 'B'
}

alpha = [{char digit} ..]
```

