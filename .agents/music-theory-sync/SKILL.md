---
name: music-theory-sync
description: Keep the shared music_theory.hpp implementation synchronized between musxdom and mnxdom.
---

# Music theory synchronization

`src/music_theory/music_theory.hpp` is shared by musxdom and mnxdom. Any change
to that header is a coordinated two-repository change.

When editing the header:

- Apply the same change to both repositories in the same work item.
- Keep the two copies byte-identical unless a repository-specific difference is
  explicitly authorized.
- Compare the files after editing with `cmp` or an equivalent byte-for-byte
  check.
- Build and run the relevant tests in both repositories before handing off.
- Keep the commits or pull requests linked so the synchronization is visible.

If the other repository is unavailable, do not treat a one-sided header change
as complete; report the synchronization blocker and identify the pending copy.
