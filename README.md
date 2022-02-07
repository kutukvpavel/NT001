# NT001

This is an incomplete linux port of an old DOS sample driver (called "meldemo") for Yaskawa Mechatrolink-II ISA/PC104 controller JAPMC-NT115.

Original intent was to try it with even older ISA/PC104 cards like JASP-NT001, that use older ASICs like JL-012 instead of JL-080/JL-098.
This attempt proved to be unsuccessful: writing a RESET command to what's supposed to be the CMD register does nothing to JL-012,
therefore JL-012 probably had a different register map comapred to older JLs (or my card is faulty, or something).

Unfortunately, no there's no publicly available documentation on JL-012 and NT001 what-so-ever.

Built with VSCode remote development, KBuild and GNU Make for Debian 10.
