# inotify Events and Mask Flags

When you use `inotify_add_watch()`, you specify a **mask**—a bitmask of events and flags that tells inotify what to watch for on a file or directory.

## Common inotify Event Bits

| Event Bit             | Description                                                                                   |
|-----------------------|----------------------------------------------------------------------------------------------|
| `IN_ACCESS`           | File was accessed (e.g., read with `read()`)                                                 |
| `IN_ATTRIB`           | Metadata changed (permissions, owner, link count, etc.)                                      |
| `IN_CLOSE_WRITE`      | File opened for writing was closed                                                           |
| `IN_CLOSE_NOWRITE`    | File opened read-only was closed                                                             |
| `IN_CREATE`           | File or directory created inside watched directory                                            |
| `IN_DELETE`           | File or directory deleted from within watched directory                                      |
| `IN_DELETE_SELF`      | Watched file or directory itself was deleted                                                 |
| `IN_MODIFY`           | File was modified (e.g., written to)                                                         |
| `IN_MOVE_SELF`        | Watched file or directory itself was moved/renamed                                           |
| `IN_MOVED_FROM`       | File moved out of watched directory                                                          |
| `IN_MOVED_TO`         | File moved into watched directory                                                            |
| `IN_OPEN`             | File was opened                                                                              |
| `IN_ALL_EVENTS`       | Shorthand for all of the above input events                                                  |
| `IN_MOVE`             | Shorthand for `IN_MOVED_FROM | IN_MOVED_TO`                                                 |
| `IN_CLOSE`            | Shorthand for `IN_CLOSE_WRITE | IN_CLOSE_NOWRITE`                                           |

---

## Special Mask Flags (Not Events)

These control how the watch behaves, not what events are reported:

| Flag              | Description                                                                                      |
|-------------------|-------------------------------------------------------------------------------------------------|
| `IN_DONT_FOLLOW`  | Don’t dereference symbolic links; watch the symlink itself (not the target)                     |
| `IN_MASK_ADD`     | Add events to the current mask for this watch (instead of replacing it)                         |
| `IN_ONESHOT`      | Only report one event, then automatically remove the watch                                      |
| `IN_ONLYDIR`      | Only succeed if the path is a directory (fail with `ENOTDIR` if not)                            |

---

## Other Event Bits (for event processing)

| Event Bit         | Description                                                                                      |
|-------------------|-------------------------------------------------------------------------------------------------|
| `IN_IGNORED`      | Watch was removed (by you or the kernel)                                                        |
| `IN_ISDIR`        | Event occurred against a directory (not a file)                                                 |
| `IN_Q_OVERFLOW`   | Event queue overflowed (some events lost)                                                       |
| `IN_UNMOUNT`      | Filesystem containing watched object was unmounted                                              |

---

## Clarifications & Practical Notes

- **IN_ATTRIB**: Triggers on changes to permissions, owner, link count, extended attributes, etc.
- **IN_DELETE_SELF**: Triggers when the watched file/directory itself is deleted.
- **IN_DELETE**: Triggers when a file inside a watched directory is deleted.
- **IN_MOVE_SELF**: Triggers when the watched file/directory itself is renamed or moved.
- **IN_MOVED_FROM / IN_MOVED_TO**: Used for tracking renames/moves within directories. `IN_MOVED_FROM` is for the old location, `IN_MOVED_TO` for the new.
- **IN_MASK_ADD**: If you call `inotify_add_watch()` on a path already being watched, the default is to replace the mask. If you use `IN_MASK_ADD`, the new mask is ORed with the existing one.
- **IN_ONESHOT**: The watch is removed after the first event.
- **IN_ONLYDIR**: Ensures you only watch directories, preventing race conditions.

---

## Example: Adding a Watch

```c
int wd = inotify_add_watch(fd, "/tmp", IN_CREATE | IN_DELETE | IN_ONLYDIR);
```
- Watches `/tmp` for file/directory creation and deletion, but only if `/tmp` is a directory.

---

## Best Practice

- **Always check which events you need**—don’t use `IN_ALL_EVENTS` unless you really want everything.
- **Use `IN_ONLYDIR`** if you want to be sure you’re watching a directory.
- **Handle `IN_IGNORED` and `IN_Q_OVERFLOW`** in your event loop for robustness.

---
