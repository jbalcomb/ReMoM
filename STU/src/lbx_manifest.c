/*
    lbx_manifest.c -- compiled-in known-good SHA-256 manifest for the original
    Master of Magic .LBX files.

    This IS the runtime's source of truth: the data-compatibility pass
    (STU_GRAF_Check_Data_Compat) checks the installed files against this table.
    There is no external manifest file to go missing, drift, or be tampered with.

    GENERATED DATA -- regenerate the rows below with the `lbx_hashes` tool, one
    run per Master of Magic distribution you own:

        lbx_hashes "/path/to/MoM v1.31 floppy"  v1.31-floppy-1995
        lbx_hashes "/path/to/MoM v1.31 cdrom"   v1.31-cdrom
        lbx_hashes "/path/to/MoM v1.0"          v1.0-floppy

    Paste each run's stdout between the BEGIN/END markers and rebuild.  Rules the
    runtime applies (see STU_GRAF_Check_Compat_Against):
      - a `version` tag containing "v1.31" marks a SUPPORTED distribution (silent);
      - a file that matches only a non-v1.31 row -> "wrong version" warning;
      - an installed file matching no row at all -> "unrecognized" warning;
      - installed files with no manifest entry, and manifest files not installed,
        are ignored;
      - files the player may legitimately edit are skipped entirely, by name, via
        g_lbx_user_modifiable in STU_GRAF.c -- currently ITEMDATA.LBX, which the
        game rewrites when custom items are created.  Leave their rows here: the
        skip is by name precisely so regenerating this table cannot reintroduce a
        check that would fire on every player who has made a custom item.

    The array is NULL-terminated (an entry whose name is NULL).  An empty table
    (just the terminator) means "manifest not authored yet" and the compat pass
    does nothing.  Currently populated from the repo's v1.31 assets/ set.
*/

#include "STU_GRAF.h"

#include <stddef.h>  /* NULL */

const STU_LBX_Manifest_Entry g_lbx_manifest[] =
{
    /* --- BEGIN GENERATED (lbx_hashes) ------------------------------------- */
    /* v1.31 (101 files) */
    { "ARMYLIST.LBX", "a155cc3a6cfab2fbd3cac3ff6e888cb98c1f07adfaea1188cd4f907da17fc4af", "v1.31" },
    { "BACKGRND.LBX", "29bdb76dc2c0cf5413fbc563098b73918efc1c68e13ced8121ccac5c86e3fbbc", "v1.31" },
    { "BOOK.LBX", "ef196f14790a22c15b9d4abf8e57cd72305df31e1d8c418ade3d5f1b97f79740", "v1.31" },
    { "BUILDDAT.LBX", "03060198570aa5ca6c06b133862994ee5c532e005da4898d1c57071c26dcc0f0", "v1.31" },
    { "BUILDESC.LBX", "842d33145f55c37f023ebe5a69c597161b2a9493680fac8d7f1a1794f3db4d2b", "v1.31" },
    { "CHRIVER.LBX", "47a6c4f201f559366ea871d8fe558152d88374c5c08b8350b9164b8e81d035a7", "v1.31" },
    { "CITYNAME.LBX", "1f66eaecb9d45c53a7575b0666cdfc7b3e963ad63a11f2603b5283a956429930", "v1.31" },
    { "CITYSCAP.LBX", "0d350956b9ee0b5ed3d254f5ed90b2e838bd0fac308033274ac3518d241952f4", "v1.31" },
    { "CITYWALL.LBX", "938a797c5bd6841cd3196cc02b63f1a43deb7f8f833c1195334aa99a485f76d4", "v1.31" },
    { "CMBDESRC.LBX", "5c2973ffe080d2e28c2c5e0022465d16fdb4f1383233e1b5f99a6ae519aee3ec", "v1.31" },
    { "CMBDESRT.LBX", "0e2da0048721ef069c73f100e3661a952740b0d48c71be53d3ec3fa90c3c683b", "v1.31" },
    { "CMBGRASC.LBX", "a63d875b308410c8df50398667a96e14ae42ca2788a8495870bb9268a234005d", "v1.31" },
    { "CMBGRASS.LBX", "c65069dbb57f9b0b9fa41c6b8b8850a367067a46864e75d44e16d54b21384709", "v1.31" },
    { "CMBMAGIC.LBX", "b4e1bb04c30bac545a22cf06f594a9adc9af0ec1302f738aede552b5880d7e2b", "v1.31" },
    { "CMBMOUNC.LBX", "4cc97d0f009d689ca70d9aeefbe4d6f8a348edad49d1915e31ab7b9bd87eee8b", "v1.31" },
    { "CMBMOUNT.LBX", "e902914c86e5533229abd0ec983c06878e40f4223bd6382a3625888b7b2f3b5b", "v1.31" },
    { "CMBTCITY.LBX", "6690c553d7c0e9496159afa37f374069a003fd2a869068f7a6c42c72d58a0e07", "v1.31" },
    { "CMBTFX.LBX", "c9f0643f66ffe7be480ad2a0fd4f1b9ce7ae9bec7f737caf6cdf1a5d88c4558c", "v1.31" },
    { "CMBTSND.LBX", "5c9d136d241c6edc66abec37c3302b3bd6126c3b405ac689d7f77e48e3f5d4d2", "v1.31" },
    { "CMBTUNDC.LBX", "a156165069467c632acda55210ab377fbfe17eaa2727e02b6fa0ea3103b36a55", "v1.31" },
    { "CMBTUNDR.LBX", "b9550f720decb7ebe78f8ae622a0065d61bcf468b37b82dde3092fc30db7c8e1", "v1.31" },
    { "CMBTWALL.LBX", "20d5b0643afd539322950c4b98e2317b8de257125a1895c0555031d51fc0a00c", "v1.31" },
    { "COMBAT.LBX", "2a6b27cc0bda37ba4e4f0a77c0a40158d921bc35d09037c9f1b11d8136411667", "v1.31" },
    { "COMPIX.LBX", "8f87b330300c197f86d1e292dddb0474d6f1982df530f3ea288a78398fe05058", "v1.31" },
    { "CONQUEST.LBX", "c560572640afc64bde29cabb8f9d1cb687936491c77a91d775262d745581ae83", "v1.31" },
    { "DESC.LBX", "961001dd3b09a5fcf387644927956ae8e5e4acb55eac5d3f663b9ffebd9d171d", "v1.31" },
    { "DESERT.LBX", "a6ebc19dfe82ee027e5d3c72992d5f6f28871d7afffc623f52ec8862f74ad623", "v1.31" },
    { "DIPLOMAC.LBX", "04cffd4aa96d266661007c55cc8ae2e8c41f91df456183bcc64b55fa1765c8b1", "v1.31" },
    { "DIPLOMSG.LBX", "4c02c445a364cffb513f2a7a67432a75877487116a6d3e129ae11ee913ed5653", "v1.31" },
    { "EVENTMSG.LBX", "3f8904713ae05c67b8455047ec4a346b56ff493f40badacc24cc6e8ad4c6204b", "v1.31" },
    { "EVENTS.LBX", "16ad42bbb30ea0f13464a3be3a478266597048dec5c1031c7bca1270c840d33e", "v1.31" },
    { "FIGURE10.LBX", "fea1b5a41d9d6400267aa20f7a4a849a2111f3b1f13333e9c3cd1a27b88bfbcd", "v1.31" },
    { "FIGURE11.LBX", "83edcebefc62f906f317ca5316fdf557f11f58a412013c7b3248e5c41c0cf803", "v1.31" },
    { "FIGURE12.LBX", "063eb78d90c3758cb577a9b1c804b00457c4eecf2d32ea826a48ad858f2bb236", "v1.31" },
    { "FIGURE13.LBX", "e30979ac3d0db1795644a26ea8d3d49d2f53201672e2313c795e6094c1762e8e", "v1.31" },
    { "FIGURE14.LBX", "3048b4e406646fc5bb434850fa3dbff7559f90cfb747739e1d9dc056d5108fa2", "v1.31" },
    { "FIGURE15.LBX", "ef99bacb682fc622eab21cee47c0d8b4e8c8daaa3deeeecc66b2c4ed8a342b0b", "v1.31" },
    { "FIGURE16.LBX", "fd1aff45718b5880bfab64459409b0b16cb0cd0b84cff23a8172807375c32502", "v1.31" },
    { "FIGURES1.LBX", "2ffe12b5872dc5eefcec5744a45573a7d39c21719874cc045d3969a010296016", "v1.31" },
    { "FIGURES2.LBX", "e7a4d39dfaa9c37eb1b597571e0f45f3867c747ef051b21c09b412087e1cba1b", "v1.31" },
    { "FIGURES3.LBX", "933fb2f0a11c35ae1bde527a84281084967b1f89a486806a77bb97b42720fa42", "v1.31" },
    { "FIGURES4.LBX", "ef2a29548341e3f97111ee69ed9f0eae1f3e864b1b4e6228d5895421b82ce74b", "v1.31" },
    { "FIGURES5.LBX", "934157e04027d3bde687a318053babcb7a86945ccf184a19a50de4808654f4b6", "v1.31" },
    { "FIGURES6.LBX", "e8f14c02b586f8a510af80f2ddebe51e6223c67dec2f99d78e0b18ccf63aa3e4", "v1.31" },
    { "FIGURES7.LBX", "f460f591ecc0fc5544b9e2202fc1abd983cfeabe3b4bb4bf1c7d6fdfe0c3e0a2", "v1.31" },
    { "FIGURES8.LBX", "217580cfe4bae727faaf201251c0541f8ca953bb0d71d8c7789e1ece5edd9299", "v1.31" },
    { "FIGURES9.LBX", "903b6b58543d1443cd971c9a241930e4a23b4833c90d9523346aceeba2db4b53", "v1.31" },
    { "FONTS.LBX", "35e61719558943ea8953fa1375abde6e41b9169e86b1e5dbf66c4c8fdf2c9695", "v1.31" },
    { "HALOFAM.LBX", "9184ee1809bc50057b01d073bcbe63dac7849a74321395d8911cb53cf250179f", "v1.31" },
    { "HELP.LBX", "a6743ddd8244edde7c175806719eb1483ec37d61b35d933bf907e7483f16b324", "v1.31" },
    { "HERODATA.LBX", "3783a963d760b7d9d9d23a8c53f68eb71b253a6545c6265d87eff2e44467191b", "v1.31" },
    { "HIRE.LBX", "39e65951a3cebb6cce5edb7e44d96f64c9f1635e94064c9b1b0b90b83b4a8699", "v1.31" },
    { "HLPENTRY.LBX", "780c3e1f25348cc49743c6f26eb8803dc44cf49198acbc7eb08d6214c7e71ae5", "v1.31" },
    { "INTRO.LBX", "3c092a724ecf301aaa818600a28923e2ea241527d35f30fd71e3fc445f71563e", "v1.31" },
    { "INTROSFX.LBX", "2b32d1ac5ff256e9a663f83391d2293203f05055d224e86aa82f248a74cf481c", "v1.31" },
    { "INTROSND.LBX", "0e1c42dd9b26ef50294bf881cf3676c6a7661f86eb0480baa0e38a914ec8aff0", "v1.31" },
    { "ITEMDATA.LBX", "7f1ea8f4fde91b47b38660670a60b37ba97a8aef026b1d4fdde5552463006c58", "v1.31" },
    { "ITEMISC.LBX", "519dd1d980b126af53f9cf35b6b0ecde8272b32e96313db49c852b1f9157b8dc", "v1.31" },
    { "ITEMPOW.LBX", "b982bc867e2ccf89598d12cf1451e469535935deb62e7ec5452c8c97499da244", "v1.31" },
    { "ITEMS.LBX", "154c20948625f744fdededcc00d318ccaf9d0db071e61d724bf903e44883f046", "v1.31" },
    { "LILWIZ.LBX", "bfc6229550d2beac2c82c682e70d24c5174a209bdce885d59204c59c5f9746ed", "v1.31" },
    { "LISTDAT.LBX", "e36caa0d56f25ab43f583de5bb482a0484900224a98fd07693841e6c0a450630", "v1.31" },
    { "LOAD.LBX", "8c1e9cbb012c6dcac9af81059813cfa13a5f1b12bef96d848429fffec5665adf", "v1.31" },
    { "LOSE.LBX", "0e775266e8522490e69b8f3c813bee468c8f28429da33ee91881177dded14ba9", "v1.31" },
    { "MAGIC.LBX", "2ceb7532d49e8c77be93e00e8c6ff9d3a8edd8830e10e34a6409128bce1cf3a3", "v1.31" },
    { "MAIN.LBX", "b1e73290557f8c640b5b021f31212ecfe91d321796a91cbc7b3b0ceda53d7528", "v1.31" },
    { "MAINSCRN.LBX", "360b5675c35cfbd64ec0b03b9f9799a9d4ea740be1a6ba7b7d79b2192c25b956", "v1.31" },
    { "MAPBACK.LBX", "8c6b97ec321c45e8bf2f0aee719ecc0d6d48dcd92bec6e1fd9c4711ebda7bb10", "v1.31" },
    { "MESSAGE.LBX", "1dc411cb43b21e2bbb7333ce89f32accbdf265349214a3a5c34a45bf653d337d", "v1.31" },
    { "MONSTER.LBX", "c4ab177ed0f029e8b8aa9918959afe260d4688d114403aefde35c38003350f29", "v1.31" },
    { "MOODWIZ.LBX", "8aac56380cb0e67e876d1ff6479d0dedd2a8f597635e5f0921bdf2b784ce69fc", "v1.31" },
    { "MUSIC.LBX", "46d74abec9a05d1665b2e58483105628f79eea200d8bd23dc581309e98fd3a9c", "v1.31" },
    { "NAMES.LBX", "5dd14f97166c81cab28546c7c3d7f39a3f4d507bf550dcff455227f02db8f83d", "v1.31" },
    { "NEWGAME.LBX", "71292c6ef0236c912cb0bc3259d43caad644e3acd5a9ce4576067353f8077711", "v1.31" },
    { "NEWSOUND.LBX", "b269c463adb2e37f5d9b2c3fa1040cca88a7d8076e48e46a8bf950d6f8d56cf1", "v1.31" },
    { "PORTRAIT.LBX", "39803ac9d29c2c58c2547ae379dbffd4f8724f137d569b8a8359a5d83b634b71", "v1.31" },
    { "RELOAD.LBX", "aedd273e410df2faf51e295a3070483ce1393cf766fc9d9e19ab48f429e90203", "v1.31" },
    { "RESOURCE.LBX", "3729f8125053e856bc525165918df443592bf2280eed2ab482cd112729620b5f", "v1.31" },
    { "SCROLL.LBX", "08ad86bca0625fbf10b64b025a2d82795b1736a20cd9004b8908ce80497a97bd", "v1.31" },
    { "SNDDRV.LBX", "37b83edaaa1a1893b7d08be0410fe48b345086798f0079898817e58121de100f", "v1.31" },
    { "SOUNDFX.LBX", "1d33b056daeeaf7cdab31b445565d9db229b9d95a4c0607dee80fdcc88271726", "v1.31" },
    { "SPECFX.LBX", "fd74f94c76572cccf21a8d752ebb36fc3fd2d0f192429ad2d2463ac7dbe1d09c", "v1.31" },
    { "SPECIAL.LBX", "e40cc26151be3287bf5878145386af093e6e3ca48c2c4e7fad9740865a75b952", "v1.31" },
    { "SPECIAL2.LBX", "1b1ef56c1d4be9db003554ad8da10e9a3071dbe9d2ce0e0db07ab2eedb0387d2", "v1.31" },
    { "SPELLDAT.LBX", "428e1fd02698ae0c1d5f48b753bb155df271f5bec10e65d32cd8ec1807926844", "v1.31" },
    { "SPELLOSE.LBX", "a7e9821387bc53d110caa9d367f20d56c23de11265975dbe5afd9a04d620916b", "v1.31" },
    { "SPELLS.LBX", "6e81ae9601f71a06cecb52a9fd532dec60ac00a4937f6d79084cc299fad2b283", "v1.31" },
    { "SPELLSCR.LBX", "cd4af88de36f05aaf5af2029c3656d330196bae78112079dee8d7940dcb1d193", "v1.31" },
    { "SPLMASTR.LBX", "8d357f0e96515eb12e5d56f7c52df7015348f9d6b0e7d56eabdb810f7969211e", "v1.31" },
    { "TERRAIN.LBX", "86e3da19cd070bb49e126e2697e95bb64f64c0cbbdb24f185eca3bf99a0fa707", "v1.31" },
    { "TERRSTAT.LBX", "7aa97ddd8079f97e339da18b690666f63b3a9480aa5afd61c0ecc00f7770582d", "v1.31" },
    { "TERRTYPE.LBX", "56ec6d39106e7aac92c0af2ee8df0a31af003c61b0e53204b7644acfe31ef03d", "v1.31" },
    { "TUNDRA.LBX", "6b335b125138218678ad9a10de49023a65f05f9fe1e92b51ebd3ee6990f20971", "v1.31" },
    { "UNITS1.LBX", "9bab1fdb23c81328c46b365bea716f26aee36b2b3a493feedd5790f52fe42adc", "v1.31" },
    { "UNITS2.LBX", "d94bd85267ffb7fe8fa9eb1cf9753a6eda7e592cf0f27d7574a50b9d8fd72dd8", "v1.31" },
    { "UNITVIEW.LBX", "bf1cc6cec1de546d9cba60b4edc5cac4330420db6698d49e9862047256055efd", "v1.31" },
    { "VORTEX.LBX", "11992c790d4c9142c06731b5493cfb2ed43baaf52e8e769c9e65b76e23843024", "v1.31" },
    { "WALLRISE.LBX", "ecacb483e9012e52730b5bf33bd2d8e2e3caf724a83fbe47daf26eb3983c2fa7", "v1.31" },
    { "WIN.LBX", "b77b249bb732167fa4549b3a574fe52bbb02037ebd9ec9ee7d67104b4838d03f", "v1.31" },
    { "WIZARDS.LBX", "b0588ef21ff50d81b25c7a107d94802d96bb952df0c86ca0f0c7842a55a09882", "v1.31" },
    { "WIZLAB.LBX", "07453eac6ca0d984c9d6c790fbba3057f1b42cc9cfc10db68a120abc0ba21c1e", "v1.31" },
    /* --- END GENERATED ---------------------------------------------------- */
    { NULL, NULL, NULL }   /* terminator */
};
