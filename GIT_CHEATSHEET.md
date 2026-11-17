# Git cheat sheet

For Windows with Visual Studio 2022. The command-line version is in section 5.

## 1. The loop after every refactoring

1. **Build.** It must compile.
2. **Run your manual test script.** The game must behave the same.
3. **Commit** (section 2 for the message).
4. **Push.**

One refactoring per commit. Push after every commit.

## 2. Commit message format

Start every message with a label, then say in plain English what you did.

| Starts with | Use it for | Example |
|-------------|-----------|---------|
| `refactor(<type>):` | restructuring. The game behaves exactly the same | `refactor(rename): rename a and b to piece and previousPiece` |
| `docs:` | only comments or `NOTES.md` changed | `docs: add seam map` |
| `test:` | only tests added or changed | `test: add failing tests for extra life` |
| `fix:` | a bug fixed on purpose | `fix: stop piece passing through the left wall` |
| `feat:` | a new feature (week 3) | `feat: show lives in the window title` |

`<type>` comes from the table in `assignment_week1.md` (Step 4), e.g. `rename`, `constant`, `inject`.

## 3. In Visual Studio: commit and push

1. Open **View → Git Changes**.
2. Check the list of changed files is what you expect.
3. Type your message in the message box.
4. Click **Commit All**.
5. Click **Push** (the up-arrow button in the Git Changes window).

Check the window shows nothing left to push.

## 4. Tags (use the terminal)

Use the terminal for tags. In Visual Studio: **View → Terminal**.

```sh
git push
git tag session1
git push origin session1
```

Replace `session1` with the tag you need:

| Tag | When |
|-----|------|
| `session1`, `session2`, `session3` | at the end of each lab session, before you leave |
| `week1`, `week2` | before the next session starts |

**Check the tag reached GitHub:**

```sh
git ls-remote --tags origin
```

Or look at the tags list on your repo page on GitHub.

**Tagged the wrong commit?**

```sh
git tag -d session1
git push origin :refs/tags/session1
```

Then go to the right commit and tag again. **If the session has ended, ask Ken or Ben first.**

## 5. Command-line equivalents

```sh
git add -A
git commit -m "refactor(rename): rename a and b to piece and previousPiece"
git push
```

## 6. Undo and look back

Throw away changes you have not committed (for example, a step that broke the game):

```sh
git restore .
```

**Warning: this cannot be undone.** Your uncommitted work is gone.

See your history:

```sh
git log --oneline
```

## 7. Push after every commit

GitHub records when each push arrives. Small, regular pushes are your evidence that the work is yours. One big push at the end is looked at closely.
