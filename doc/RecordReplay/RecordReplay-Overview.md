
SEEALSO: C:\STU\devel\ReMoM\doc\RecordReplay\Handmade Hero Day 023 - Looped Live Code Editing - Video Transcript.md



--record
--replay
--demo



./platform/replay/Replay.c




## --record
ReMoM.c
    if(strcmp(argv[argi], "--record") == 0 && (argi + 1) < argc)
        Platform_Record_Start(argv[argi]);



## --replay
ReMoM.c
    if(strcmp(argv[argi], "--replay") == 0 && (argi + 1) < argc)
        Platform_Replay_Start(argv[argi]);




Handmade Hero's record and replay system, covered in Day 023, enables live code editing by recording keyboard/mouse inputs and saving a base memory state to disk. By restoring the memory state and replaying the recorded inputs, the game can instantly reproduce bugs or test changes, speeding up development iterations.

https://hero.handmade.network/forums/code-discussion/t/6875-save_playback_input_with_sdl#:~:text=Input;,that%20you%20are%20playing%20back.

https://www.reddit.com/r/gameenginedevs/comments/j56s4q/handmade_hero_episode_guide/#:~:text=It%20works%20as%20long%20as,iteration%20time%20really%20really%20fast.&text=That's%20a%20really%20great%20design,screens%20of%20your%20engine%20btw!

https://www.youtube.com/watch?v=xrUSrVvB21c&t=1825s

