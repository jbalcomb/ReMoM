
Handmade Hero Day 023 - Looped Live Code Editing - Video Transcript.md

https://www.youtube.com/watch?v=xrUSrVvB21c&t=1825s
Handmade Hero Day 023 - Looped Live Code Editing
Molly Rocket
42,633 views  Dec 18, 2014
Day 023 of coding on Handmade Hero.  See http://handmadehero.org for details.

Transcript - English (auto-generated)

0:00
and then a start recording. There we go. Uh, welcome everyone to Handmade Hero.
0:05
We are here on day 23, uh, which basically maps to 23 hours on stream and
0:11
we are closing in on the end of our, uh, work with the Win32 platform layer. We
0:17
finished substantively all the stuff we needed to do last week. Um, but this week we are basically spending a little
0:23
extra time to do some fun uh, and pretty powerful stuff with it. So the past two days I showed how you could um have your
0:29
C code be dynamically reloading all the time. So that basically you never have to close the game and reopen it while
0:35
you are tuning. You can just go ahead and update your code and it live updates directly. So like basically just like
0:40
everything you might want from a scripting language, you can just get straight ahead with CC code. Uh which is pretty awesome. And we and uh we played
0:47
around with that a bit and it's working really well now. Uh it works under the debugger and it reloads instantaneously.
0:53
So I'm pretty happy with how that turned out. Um, but today I wanted to show how to do something even cooler,
1:00
uh, that I've actually never seen done before. I mean, I've done it before, uh, but I've never seen anyone talk about it
1:07
or mention it. So, I don't know if other people are doing it and I just have never seen their writeups or if maybe they're doing it and they just never
1:12
told anyone. Um, but I'm kind of excited about it because I don't think anyone's ever actually presented it. Maybe not.
1:18
Uh, we'll see. Uh, but it's it's pretty exciting, I think, because it's a really cool feature that you can do very easily
1:24
if you've architected things properly. It's actually very difficult to do if you haven't. Uh, but since we've architected things properly, it's going
1:30
to be very easy for us to do, I suspect. So, um, with that in mind, um, welcome
1:36
to Handmade Hero Day23, where we code an entire game from scratch using no libraries, uh, and no engines or
1:43
anything, just so we can see how absolutely everything, uh, works in an entire game stack. uh which I think is
1:48
very educational and also very empowering because it means you can always write anything you need to even if you are using an engine or a library
1:54
you can drop right down in there and fix stuff if you need to or write your own stuff if uh the engine falls short or
1:59
something isn't working and it just allows you to understand uh how engines are working and it allows you to better
2:05
uh sort of manage them from your side of things. So even if you are using engines or libraries knowing how all this stuff
2:11
works under the hood can be a very very powerful tool uh when the time comes. So, um, with that in mind, if you have
2:18
pre-ordered the game, uh, you should have gotten a link in your email where you can download a zip file which has every single day's source code, uh, in
2:25
there. So, if you want to, since we're on day 23, you can unzip day 22's source code. And when you do that, um, you will
2:32
be in the exact same place that I am right now. Okay. Uh, so I'm going to go ahead and jump right into it because we
2:38
have a lot to do today to get to the thing that I, uh, want to show you. So,
2:44
let's get started. Uh basically what I want to point out now is we have what we
2:49
did it was a pretty darn cool uh we basically uh do have you know most of
2:54
what I would want um for something where you can do live code editing right I I can run the game um and we've basically
3:02
shown this sort of simple we don't really have uh since we haven't been working on the game we've just been working on platform code and stuff like
3:09
that and and all the sorts of things that we need to do to support the game on Windows um we basically haven't had a
3:15
to do much in the way of an actual game. But uh of course we have demoed the fact
3:20
that we can live change anything uh that we want and it immediately updates uh pretty much instantaneously upon change
3:27
uh in the editor. So that's pretty awesome. But uh if you think about what happens uh in a game usually, you know,
3:33
like we've just got this kind of toy thing running over here, but pretty soon uh you know, over the next month or two, we're going to be building actual game
3:39
stuff and there's going to be a real game running in here and things won't be so simple in terms of actually being
3:44
able to see the results of code changes, right? Um basically what's going to
3:49
happen is we're going to have user input and they're going to be moving a character around and they're going to do
3:54
stuff, right? Uh, and so I just want to quickly create a little very simple uh,
4:00
example of what that's going to be like just to point out uh, where we're going to run into problems with this live code editing. And to be fair, it's not
4:06
anything wrong with our live code editing. Our live coding at this point is as good or better than mostly anything I see out there in in other
4:12
stuff. Uh, so our our live code editing is good right now, but I want to make it way better. I want to make it so much
4:18
better um, that it's going to be like you you would not dream of using something else after you saw this, if
4:23
that makes sense. Um, but in order to demo why this is going to be cool, I just need a little bit of something that uses some user input or something so
4:30
that we can kind of see what happens when you actually want to um tune something in a game. So, I'm going to go into here to our sort of uh standin game
4:37
code that we did that's just sitting here uh waiting for us to start writing a game in it. Um, and what I'm going to do is inside uh you remember we have our
4:44
game state thing where we store some state. I'm just going to add like a very simple player X, player Y. Um, just just
4:51
so we have like a location uh where we're pretending the player is, right? Um, and then I'm just going to flat out
4:56
say, all right, when we come in here and initialize that, I'm going to pretend the player is somewhere. I don't know
5:02
where. He's at 100 100, let's say, right? Just just something real basic like that. Um, so the player starts out
5:08
at some random location on the screen. Again, we're not thinking this through. It doesn't matter. This is just a simple uh example. And then what I'm going to
5:14
do here is I'm going to do inside of our um our code for the keyboard handling or
5:20
the non-analog handling. Basically, uh I'm just going to go ahead Well, you know what? I'll just use that that stick average X and the stick average Y. So,
5:27
I'm just going to go ahead and uh for any controller, no matter what it is, I'm just going to go ahead and take the
5:33
player X and the player Y and I'm just going to add to them whatever these stick values are. Like it just again, I
5:40
don't even care what's going on here. I'm just gonna I'm just going to do that straight away. And then when we do render weird gradient um I'm also going
5:46
to have a a a ridiculous function um called render player. This is like
5:52
player in real big quotes here. Uh which will basically just be um excuse me uh
5:58
something that draws anything whatever we can think of uh to the player location. Now of course we already have
6:04
some code in here and we'll be writing our own rendering code very soon so we won't have to be so uh slapdash and
6:09
sloppy about this. But again, so you know, one thing at a time, we got to go in order. Uh, so basically we have this
6:14
this win32 debug draw vertical thing, right? Uh, which basically just kind of goes through here and does this um does
6:23
this nonsense, right? Uh, basically what it does is it it uh it it'll draw one
6:28
vertical line and we were using it uh to draw these these hash marks, right? Uh, so what I'll do is I'll just do
6:34
something which basically you know basically does that. I'll just do something which which uh which uh let's
6:40
see here. Okay, there we go. Uh something that basically
6:46
just loops through the x and y. In fact, you know what? Now that I think about it, um I don't know if this would be easier. Well, now probably not. So,
6:52
basically, I'll just do something uh that loops over some x's, right? Um it'll start, let's say, at the player x.
6:59
Um it'll go to the player x + 10 or something like this. Um and it'll add uh
7:05
to x. So just just the most basic thing you could possibly imagine. Um, and this thing uh will basically be uh top will
7:12
be player Y and bottom will be player Y + 10. So I'm just making a simple 10x10
7:19
rectangle. Uh, and I'm going to go ahead and fill that stuff in. Now, this will full-on crash if we if the player moves
7:25
outside the window. So this is this is the like absolute worst code you could possibly imagine. Okay, this is never
7:31
code we would even dream of putting in the app. I'm literally just slapping in there because we haven't written a render yet and I just need to put
7:36
something on the screen. So, uh I need to to uh basically pass in a player X
7:42
and a player Y here. Um and then let me take a look at uh actually what uh I
7:48
don't actually remember what these um these calls are here. So, it's buffer
7:54
memory. Uh we're going to need buffer bytes per pixel. Although, we actually don't even need that at this point
7:59
because um I don't know if we're going to basically fix that value. This is this is basically four here, but I can
8:04
go ahead and add that to our thing. Um, since seems like it might be nice to just have it there for code that could
8:10
work either way. Um, let's go ahead and do that. So, I'm going to go in here where we uh where we pass that buffer to
8:17
the game. And I'm going to go ahead and add that in there. So, this right here,
8:22
um, buffer will be bytes per pixel. Oops. Bytes per pixel. Bytes per pixel.
8:29
Um, and then I'm going to take a look at compiling this. All right. Uh, so the color for the player, I don't know what
8:35
color the player should be. Uh, maybe the player will just be a white uh, rectangle for now. Uh, the back buffer
8:41
is just buffer at this point. Um, render player now needs to get called by
8:46
somebody. So we go down here. Uh, and we will just do it right here to render the
8:54
player. Okay. Uh, so basically just a quick, you know, five minute thing here where we actually have something going.
9:01
So I don't know if this will work. I just kind of slapped it in there. But, uh, there is my player. See it right
9:06
there. Um, and I do not actually know if I can move him. Okay, I can. So, I have
9:14
my gamepad here. Uh, and you can see that I can move my player around. Of course, it's a little wonky because I've got the stick mapped wrong. Let's Let's
9:21
go ahead and I didn't think about that. uh the the y direction of the stick. Uh
9:26
up is actually um up is positive and down is negative. So we're going to have to go ahead and re reverse that there.
9:33
Um but yeah, so uh I've got this little guy and I'm
9:38
moving him around, right? Uh so basically just imagine that this and and I should probably uh I should probably
9:45
turn off the gradient motion and and for for God's sake turn off the sine wave. Um, so maybe let's just say uh that game
9:52
output sound from now on uh let's let's go ahead and and uh and pretend that
9:58
we're we're just quiet. Uh we just we just don't need all this all this sound happening. So let's go ahead and uh and
10:05
and basically um uh do this here. There we go. So instead of computing that sign
10:12
value, let's just do let's just do quiet because that's going to get real
10:18
annoying. Uh, all right. So, anyway, uh, here is our little here's our little guy, um, who's moving around. Why did
10:25
that, uh, that was a little odd. I feel like that's a little odd. There's a bit of a frame rate. Did I Did I do
10:31
something stupid there? I don't know if I did.
10:36
You can see I don't know if you could see that. Uh, but basically, I was getting a weird kind of frame rate hitch happening there. I don't actually know
10:43
why it is. I think that it's weird because our game still is running at 33 uh milliseconds
10:51
per frame. All right. So, we'll have to take a look at that a little bit later. Although, that may again be sometimes
10:56
OBS uh creates a lot of problems for frame rate stuff. So, it's hard for me to always know whether it's something we
11:02
need to look at or not. But, basically, all I did is right, I just made a little rectangle here that I can move around the screen. That's it. Um, and so what
11:08
I'd like to do is I'd like to be able to um I just I just needed something where
11:14
I could show sort of what a game would normally doing so I can show what's difficult about um about tuning code. So
11:21
let's say right that what I want to do now that I've got this is I want to implement a way that this dude can jump
11:28
or something like that, right? I want to have it so that when you push the button uh the dude jumps. And so I'm going to
11:34
go ahead and just implement something simple there like if action down, right? We've got this action down thing happening. Um so if action ended down,
11:42
um then we're also going to pretend that the player, you know, jumped here. We're going to move we're going to discontinuously uh move him up the
11:49
screen uh by 10 by 10 pixels or whatever for right now. Right? So I've got my
11:55
guy. I move him around. I hit the button and you can see when the button goes down, right? uh he jumps, right? And he
12:03
doesn't come back down or anything, right? I'm just showing you kind of what the deal is. So that's it, right? Well, let's suppose now that we want to work
12:10
on our code for the jump. So you can tell that the jump is ridiculous. It's totally stupid. In fact, all the player
12:16
movement code is awful. Um so what I would have to do if I was doing this live, right? If I'm if I'm if I was
12:22
tuning this code, even with our live code editing, here's the process that has to go through it. All right. Well,
12:27
what happens with the jump? Well, I pushed the jump button and he doesn't come back down. Uh, so I guess I need to
12:32
add like a timer or something um in here or something like this, right? So, I'm going to have a timer uh in the game
12:39
state. I don't know uh something that's like um maybe a velocity, right? So, I've got a
12:46
a a player velocity in Y or something like this. Uh and I'm going to set that to something and then I'm going to
12:51
change it over time. Or maybe I just have a timer. So, like I just have a T jump timer. I don't know, something like that. Um, so then I come in here and I
12:58
go, "All right, the T jump timer, um, is going to be, I don't know, the game state when he jumps, the T jump timer is
13:05
going to be equal to 1 second." Um, and then basically like, you know, I don't know, we add the sign of the T jump
13:12
timer. Um, kind of a thing like this. I don't know what's going on. Um, and I do
13:18
that and then every time we go through the frame, um, I subtract something from the T jump timer. You know, I don't
13:24
know, something like this. this. I don't even know if we're passing the uh the um frames per second in here or anything
13:29
like that. But you get the idea. Like I'm going to I'm going to have to do um a bunch of coding in here. Uh let's see
13:35
here. I've got to actually add that inside there. So I'm just I'm going to have to do a bunch of weird coding in here uh to make this jump code actually
13:42
do something. Um so when I'm when I'm actually doing it, I have to then make those changes. Come back in here, push
13:48
the button. Okay, like that didn't do anything, right? Um and of course it makes sense it didn't do anything because I'm not multiplying this by
13:54
anything. So, I have to come back out here. Uh, and I guess since we're doing the live coding, I could leave it
13:59
running, right? I can leave this guy running and that's fine. But when I come back in here, I have to do like, okay, I
14:04
edit it, right? Recompile. And now the code is live or whatever. But, oh,
14:09
although we have a Oh, I may have to actually make this guy uh
14:17
I may have to actually prevent this guy from running out of bounds. Like I said, this guy will write out of bounds whenever the the the player actually
14:23
gets to the bottom of the screen. So, I may have to do an actual um an actual attempt here at preventing this guy uh
14:30
from actually writing out of bounds. So, uh let's just do something really simple, which is uh if pixel if if the
14:37
pixel pointer is is in bounds, it will do it. Again, this is absolutely horrid code. Uh, I apologize for for being so
14:44
horrid on the stream, but I just want to I'm literally only doing this um uh to
14:50
to prevent to to to show kind of why we want the thing that I'm going to show you how to build. Because if we had
14:55
already written the game, then it would be very easy to show or had some part of a game running, it would be very easy to show uh why the thing that I want uh to
15:02
do is interesting. But without um having any of that, it's much easier. All
15:07
right. Uh sorry, it's much harder. So I I need something if that makes sense. All right. So basically we have the
15:14
buffer memory here. I'm just going to add the full size of the buffer in there. Um so the bytes per pixel uh
15:22
times the width plus the pitch times the height. So I'm just going to add in
15:29
basically the entire size of the buffer. Again, do not worry about this code at all. I will be explaining renderers in
15:35
detail when we get there and I just don't want to belabor the point at the moment. um because it doesn't matter.
15:40
All right. Uh so anyway, here we go. Uh you can now see that my jump code,
15:45
awesome as it is, isn't even waiting for the timer to be set, right? Because it's just always working. And basically what I had to do is make it so that when this
15:51
thing goes out of bounds, we don't crash because when it would go, it would be writing out of memory when we go here, right? Um so all right, so I'm tuming
15:57
this. So I I do this thing and I'm like, "Oh, right. Uh the timer isn't is totally messed up. It's it's it's not uh
16:03
right at all because it keeps going down." So I I I flip back here or whatever. I go, "All right." Um, you
16:10
know, uh, so we got to do this this, uh, game t jump thing. Uh, but that should
16:15
only be happening, um, basically if t jump is greater, uh, you know, is
16:20
greater than than some threshold value like, um, you know, so basically what I want to do here is say, uh, if game
16:28
state t is greater than zero, then we will do, you know, this piece of code or
16:34
whatever otherwise we won't. So, you know, we'll do that. Uh, we we'll we'll
16:40
do something like this, right? There we go. Um,
16:45
and there. Uh, so I do that. Okay, I got an error there. Got to convert this to an int. Done. So, I come back here and
16:53
now at least, you know, it's a little bit better, but I got to pick up the controller again. I got to get back to somewhere I want to test. I got to push
16:59
the button. Oh, and the jump goes downwards, right? Not at all what I wanted. Sign goes downwards. Okay. All
17:04
right. So, stop. Flip back here. I gotta go ahead and say, "All right, this should be going the other direction. I
17:10
run it again. Come back here. Pick the controller back up. Take my hand off the keyboard. Do it." Right? So, you can see
17:16
what I'm basically trying to say here. There's this whole process where I have to keep going like ah like this is a
17:23
problem because I got to like pick up the controller, test the thing I want, go back to the code, change the code,
17:29
come back here. And you can imagine if there was more state, like if there was an enemy, what if I killed the enemy? Well, now the enem is gone. And so that
17:36
firing the missile and what if the missile only had I only had one missile left. Do I add a cheat key to add more missiles? There's all these kind of
17:42
annoyances in there that make it not as smooth as I would want it to be, right? Uh so what I would like to do instead is
17:50
make it so that this can basically be like the way musicians edit stuff, right?
17:57
I basically want to create something that's sort of like a loop editor where I can just go ahead and say, "Okay,
18:05
I'm going to record something being done." And then I want you to just keep doing that thing over and over again
18:11
exactly the way I did it as I tune the code for it. Does that make sense? Um,
18:19
so you know, I just I want to make something that's essentially like a loop editor but for code. Uh so how do I do
18:25
that? Well, it turns out like I said, if you architect uh everything properly, this ends up being actually extremely
18:31
easy because think about how we have set up our game there. Uh if we don't care about sound for now because we have a
18:37
separate s way that sound comes in, but if all we're trying to tune
18:42
is uh is stuff besides the the literal sound output code, we only have one
18:47
function that's called, right? It's called update and render. We know it takes the memory that's whatever the
18:53
state of the game is. We know it takes whatever the input is on this frame and then it just takes wherever we want it to draw to, right? Whatever we want it
19:00
to draw. That's it. So if we wanted to,
19:05
we could certainly start doing something relatively straightforward here by saying, all right, what if we just
19:11
record whatever this new input thing is? We just remember whatever that is, right? And then we feed it back. Instead
19:20
of actually producing the input uh before we pass the thing here, we just actually see whatever the previous input
19:26
thing was, right? Um we just record it and go. Seems pretty straightforward,
19:32
right? So if we want to uh we could basically introduce something, right, that captures this input. Um because we
19:39
know it's just a flat strct. We made it very simple, right? It's really trivial to to capture. It's just one of these,
19:45
right? So you could imagine uh having something uh that was like you know
19:50
wind32 recorded input right you could imagine us constructing something uh that looked like this right uh there's
19:58
an input stream right something like this uh and there's an input count right
20:05
and it would basically say all right as we go we just remember all of the the inputs that we had we stuff them in here
20:11
and then later when we want to play it back we just go ahead and grab them out of here progressive ly,
20:19
right? Very, very, very simple. There's really nothing weird going on about that at all. Furthermore, if we wanted to, we
20:27
already know how to do file IO. Um, so if we want, we actually don't even really need uh to do, excuse me, we
20:35
don't even really need to store it in memory, right? We could just log it out to a drive uh and save it that way so it
20:41
was permanent, right? We could just read and write from a file. Uh so basically
20:47
there really wouldn't be very much to at least that part, right? So let's think about how we might implement this. Uh
20:55
all I really need to do is sort of know whether I'm recording something, right? Uh so maybe I'll have a variable here
21:01
and you know I might want to record multiple of these or something. So maybe I could just introduce a variable here
21:07
called like uh recording index or something and recording index will be equal to zero.
21:14
uh right uh so when we're running we also have a recording index and that recording index is equal to zero and
21:19
similarly uh if we were playing something back maybe I'll have a playing index um uh like an input recording
21:25
index and an input playing index right and basically if these are equal to zero then we are not playing or recording but
21:31
if they're equal to uh something else then we're we'll think of it as like a slot basically uh that things are in
21:38
make some sense it seems pretty straightforward right I don't think there's anything particularly odd about that uh So, if we want to now, uh,
21:45
basically what I can do is I can sort of, um, uh, well, I guess there is one
21:50
we we do have a bit of a thing that's a little annoying here, which is that, uh, we kind of have our keyboard processing
21:56
up here, and we'd like to start using that, uh, keyboard processing to affect some stuff. So, it would kind of be nice if we had these things sort of starting
22:03
to be centralized and something. So, maybe, you know, I'd like to do something like this, which is wind32 state or something like this, uh, where
22:10
I where I have something like that, right? and uh and that has some of these sort of globally sorts of things in it.
22:16
So I have win32 um state and that's here and so I'm just going to go ahead and and wrap those in a strct so that I can
22:23
pass them around because basically what I want to do is I basically want to excuse me basically I say basically a
22:28
lot uh what I want to do is I want to make it uh so that we can record things by just hitting a simple key. So
22:35
basically uh I I look to see if we have hit the L key let's say for loop right
22:42
um and uh what we'll do is we'll say we'll take a look if maybe if the shift key was down um I don't know if or or
22:49
what what was the easiest thing to do there was the uh the alt key was down so we'll go ahead uh and well actually we
22:55
don't even need that I don't think we need that at all uh if you press the L key that'll go into loop mode let's say
23:00
uh and then if you push um one of the number keys uh that will indicate what
23:06
slot you want to record in. So we'll basically say we'll push L and then one will like start recording input to slot
23:12
one. Um and then you push L again and it stops the recording at that point, right? So L1 starts recording, L stops
23:18
it and then from then on if you hit one it'll uh start replaying that slot, right? Uh so let's go ahead and do it
23:26
that way. Um for now I guess what I can do is just hardcode it so we don't have to do the one two three processing
23:32
because I can show you how to do that in a second. Uh but so let's just say that if you hit the L key uh then the win32
23:38
state um the the input recording index right uh becomes one uh if it already
23:46
equal that so if the if it equals one already or if it equals zero it's that otherwise we we stop right otherwise we
23:53
will um we'll set it back to zero. So, basically, we're just making a toggle here. We hit L, we start recording, we
23:59
hit L again, we stop recording. And since we're testing here, we'll switch right to playing back as well.
24:08
Make sense? All right. Pretty straightforward, I think. Not much to to do there. Um, all right. So, basically,
24:14
I've got these these uh playing indexes and and that sort of stuff. Move those into here. Uh, and so
24:21
basically we just replace those uh with with our own struck there. And we can start
24:27
putting other stuff in there um as well. So this will kind of be a nice holder for things that we want to be accessed
24:32
outside. So I go ahead and do that. And now all I really need is I need to uh
24:38
look at those when we're about to use that input. Right? So right here we have new input. And what I want to do is I
24:44
just want to say well if the win32 state, right? um
24:50
has a uh input recording index, right? If it's on, if recording is on, uh then
24:56
I want to call something like wind32 uh record input like this. Uh and I want to
25:01
record this input that just came in. Um and if the wind32 state is is uh playing
25:07
back something, right? Uh then what I want to do um is uh is play back uh the
25:14
input. Uh, and so I'm going to overwrite whatever new input was with whatever the input is, uh, whatever I'm basically
25:22
going to get, excuse me, whatever that thing reads from the stream of previous
25:27
inputs, it's going to put it in there.
25:33
Very, very straightforward, right? So, we've got these two functions.
25:39
Really, really simple. Uh, again, nothing fancy happening here. Uh really all we have to do is implement these uh
25:47
very straightforward. So I've got my win32 state. I'll have to go change that to a pointer down there, won't I? Uh and then I've got my game input uh
25:55
that's coming in here. So we've got those two things. Uh that's our our record input. Then I've got another one
26:01
here that's going to play with the input back 32 state. Uh this and game input
26:09
new input. So there we go. And all I need to do is I just need to either
26:15
write in the record function, I just need to write it to a file. In the playback input, I just need to read it from a file, right? That's it. Um, so
26:23
assuming that I have a file handle, which I could totally do, right? I'll have a file handle here, which is
26:28
recording handle. Um, and I'll have a file handle here. Oops. Uh, which is
26:34
playback handle. And we'll have to actually open these these files, uh, with with Win32 at some point, but I've
26:39
got those. So, I'll go. All right. Uh, I've got this recording handle. So,
26:44
basically, all I need to do is just write this out. Uh, and we already saw how to do this. Remember, we have our
26:50
write file. In fact, we uh we already wrote a thing that writes an entire file. It was just this, right? Um,
26:55
that's that's the entirety of it. We just need the file handle. Um, so we need whatever the file handle is. We need the memory, which is the new input,
27:02
right? We need the memory size, which is just the size of that strct, that new input strct. Uh, and then we we don't
27:09
even really care about the bytes written because if it fails, it's just a bug code. We don't even care if it fails. Um, so just we're just going to try to
27:14
write and hope that it writes, right? So that's it. Uh, so that's all we really need to do to write it. So we write that
27:20
to the recording handle, right? That's it. Um, and then the same is the exact
27:26
same thing happens uh with the read file, right? Read file. Uh, where is
27:31
that guy? That's the wrong function. There we go. Uh so read file
27:39
same thing again. Uh so we basically have our our bytes read. Um we don't really need to look at it right now. We
27:45
will have to eventually when we want to get the looping stuff working but that's that's you know in a second. So wind32
27:51
state playback handle I think is what I called it. Yeah playback handle. Uh and so then
27:59
we just need to do the exact same thing here. We pass the new input. That's what we're writing. uh we pass the size of
28:04
the new new input um and and we're done, right? So this is literally all we actually need to do uh to record and
28:10
play back the input, right? That's going to happen. And then uh we also have uh
28:17
the the case of when we actually first start recording. Uh so we basically have to do essentially something that goes
28:23
back to the beginning of the recording or opens, you know, opens whatever this file handle is. Uh that sort of thing.
28:28
So, uh, we need something that's basically like begin recording input or
28:34
something like this, right? And then we'll need something also that's like when 32 end recording
28:40
input, right? We need something like that. Uh, and these will basically just be the things that get these file handles in the first place. So, to begin
28:47
recording input, uh, we're going to just go ahead and do a create file just like we did before, you know, to open the
28:53
file handle with Windows. Say, hey, let's let's get this back. So, there we go. Uh, so we need to open our recording
28:59
handle here. Uh, we're going to go ahead and create file. We want generic write privileges this time because we're
29:05
actually writing to the file. In fact, I should copy the create file that we did uh for for the other one because that's
29:11
the one we actually want to make a file here. Uh, so we have our our file name for now uh will just be fooandmade
29:21
hero. Let's say I don't know that's probably a bad name. Handmade recording handmade input. I don't know. Uh we're
29:27
going to deal with that in a second obviously. Uh so we got our file name here. I never can decide whether file name should be capitalized inter caps or
29:34
not. And I use it interchangeably in the code. It drives me nuts. I wish I had a consistent way of doing it. Uh so
29:39
basically we go ahead and do the recording handle. We open that up. Um that's pretty straightforward. Uh and
29:46
and we can even go ahead and and say uh that recording uh we'll pass that input
29:52
recording index here, right? and we'll say win32 state input recording index equals input recording index. We'll do
29:58
the open and then in here we'll do a uh close handle on that and then we're
30:04
done. That's all we need to do. Uh and so
30:09
basically these guys uh we'll we'll move up here to above. I'm putting them above
30:14
because we're going to call them in here and I don't want to bother having to forward declare them. things you
30:20
wouldn't have to worry about if you were programming in John Blow's new language because you don't have to worry about any of that stuff anymore. So, that's
30:25
going to be pretty awesome when that happens. But, yeah. Uh, so Win32 begin recording. Um, it's going to begin
30:31
recording input one. Uh, and this will be Win32 end recording input. Uh, and
30:37
then it'll be Win32 begin um input playback and it'll also be one.
30:47
There we go. Uh so yeah, so that's really all we need to do when we hit L. If we were weren't recording something,
30:53
we ask it to start. Uh otherwise, we ask it to stop recording and then to start doing playback. And so, you know, we've
31:00
got uh we've got to do the equivalent of these
31:06
two functions. Um and
31:12
all right, there we go. Back playback. That's another intercaps one.
31:18
Should it be playback or playback? You know what I'm saying? You know what I'm
31:24
saying? I don't know. See, and I've done it I've done it two different ways. That's just It's maddening when I don't know how it should be. I feel like there
31:31
should be some kind of a style guide that tells you where there are the word breaks in playback, file name, that sort
31:36
of thing. Uh and so these are actually wrong now because uh again for uh
31:43
closing the handle on the end of playback is fine but we do have a problem where our uh create file is the
31:49
wrong kind of create file that's creating a write. So we want to copy again the the one that we used for reading the file. This one right here.
31:55
There we go. Uh and and that should be uh fine because we're always going to open an existing one uh for our playback
32:03
handle. Uh, so that's about it for those guys. Uh, so
32:10
if you think about what that's going to do, right? Um, when we hit that L key, we should start just recording in uh our
32:16
input to this food.htm HMI file. Uh, then when we we stop that, we're going to start uh reading it again. Now, when
32:23
we're actually doing the reading the input uh in our in our like in our playback uh phase where we're actually
32:28
reading stuff, we do want to know when this starts to fail. um because when it fails, we want to go back to the
32:34
beginning if we're trying to loop, right? Uh so what we would want to do here is we'd basically want to do um a
32:40
win32 end input playback uh and then win32 begin input playback, right? So we
32:46
just want to do whatever the wind32 state um excuse me
32:52
uh whatever that uh that input playing index is, we would basically want to store that. Uh so this is our playing
33:00
index. We basically want to just pretend that we stopped even though we really aren't stopping so that we close down
33:05
the handle. Uh and then we can go ahead and and start again. And we could do it
33:11
a little more efficiently than that if we wanted to, but I don't think we do because there's a bunch of stuff that has to happen during begin that we'll
33:17
see in a little bit. So basically, I just want to set us up for success there. Uh basically by by making this do
33:22
it as if it ended and started again. All right. Uh so and there's my that file
33:28
name intercaps, right? It's crazy, right? It's it's it's awful. Uh, all right. So, basically, uh, we're we're
33:34
almost done, surprisingly. Um, for such an awesome feature, you're going to be
33:39
shocked at how little work this thing is. Um, all right. So, uh, we've got our
33:45
Win32 state coming in now. Um, we've got our, uh, record and playback happening
33:51
when those things start and end. Um, playback input. Um,
33:57
what is wrong with that? Oh, that is win32 playback input. I guess new input
34:03
is already a pointer. That's the difference. So, uh there is a couple little things. Uh again, I slammed that
34:09
in there real quick. I just want to uh take a quick second when we do begin recording input. Um this is supposed to
34:15
be input playback ind playing index, right? Um oh, I like when people on the
34:21
stream say copy pasta. I I've never really heard people say that phrase. It's kind of funny. um when you cut and
34:26
paste some code and you're supposed to be changing it to be some different code but you don't. It's kind of funny. Um
34:31
anyway, uh so I'm passing this in input playing this when I close these out when I do the end uh recording. Uh I need to
34:38
basically clear out those handles as well. Um right. So I need the input playing index to to change to zero. Uh I
34:46
need the um the input recording index to change to zero when you when you end
34:51
like that. Um, and I think that's roughly everything that I wanted. All right. So, let's take a look at at what
34:58
happens here um when we do this. So, I'm going to go ahead and just set a
35:03
breakpoint um inside uh that that keyboard handler basically just so we
35:08
can see what happens. So, uh here we go. I'm going to run the game. I'm going to hit the L key to start recording. Uh
35:15
we're going to take a look here at our win32 state structure, which basically should be just completely cleared to zero, which it is. Again, I always like
35:21
zero to be initialization. So, you just don't have to worry about initializing stuff. You just clear it to zero and you know that it's good.
35:29
All right, we step in. Uh, we say, "All right, it's time to begin recording. So, we're going to go ahead and set the
35:35
recording index to whatever slot we're recording into. Uh, we're going to go ahead and try and create a file. We
35:40
created the file. Off we go." We pop out of here. Blah, blah, blah. Nothing particularly interesting going on there.
35:46
We go through our code and we get down to the point where we've done all of our input processing. We say, are we
35:53
recording? We are recording. So, we step in here. Uh, we take a look at this write file. It looks like we're writing
35:58
to a valid handle. We're writing out the input thing. Everyone is happy. We wrote 600 bytes. Is that how big our input is
36:05
for a frame? Um, we can check uh size of game input 600 bytes. That's exactly
36:11
what it should be. Everyone's happy. Uh, we come out of here. Uh, we keep going
36:16
and it's good. All right. So, basically, this is writing off out that stuff to disk, right? This is writing out out
36:23
everything that I'm doing. Uh, so if I hit L, uh, in theory, this should now change to ending the recording input,
36:30
which it does, closes the handle, uh, goes in here, uh, goes to start playing it again, creates that file, uh, and off
36:36
we go. So now uh we actually should in theory uh be playing back whatever the
36:42
user input was uh for for all the frames. Now I don't know if it's actually working and of course I had
36:49
left my guy sitting there for a while. So we we would have to actually wait for a bit to see if this guy moves. But I
36:54
also want to take a look um I I really wanted to kind of look and see uh how
37:00
that input code uh was actually working when it does the readback. So, I'm going to go ahead and uh do this in a test
37:07
where I know that I'm doing something when I hit that L key. Uh so, basically
37:12
L key. Um ah
37:19
oh, stupid mistake. And this may have cost us from having it work the the right the first time. I I totally forgot
37:25
to check whether the key is going down or up. I always forget that when I jump in here. Uh so, basically, it was it was
37:30
triggering twice. Once for the down, once for the up, which meant it started to stop recording immediately, and that's not very much fun. Um,
37:37
all right. Uh, so basically, here we go. Let's try this again. I do the L key. I
37:43
do the jump. I come back down. I push the L key. Hey, look. Uh, we're playing
37:48
back our input. I don't know if it's actually working properly. It doesn't look like it recorded the jump. So, I think we're still a little bit buggy.
37:54
Uh, but at least it worked slightly. So, let's go ahead and and uh and take a look at what what I messed up because I
38:01
think we're almost almost there. So, uh all right. So, when we when we do our
38:08
recording, we are supposed to be writing write file. Uh we're supposed to be
38:15
writing this every time with our win3 record input. And then when we do our playback, uh we are Oh,
38:24
what? What? What did I type? How did this happen? What? This is This is what
38:30
happens when you explain stuff uh when you're trying to do it. You just you're you're you become an awful programmer,
38:36
everyone. I I I promise you I'm not as bad at programming as I appear to be on this stream. Like, if you actually look
38:42
at the stream, everyone's going to not want to hire me. They're going to be like, "You're the worst programmer." Like, why would this How did this get in
38:49
here? This this this is this is this is totally wrong. Yeah. Okay. So anyway,
38:55
basically what we did is if read read file succeeded, we then started looping. That doesn't
39:01
make any sense. Obviously, we only want to start looping when the read file is finished. Uh I mean that's kind of
39:07
obvious, right? And read file, if I'm not mistaken, returns a bool. Um so it's going to be true uh when it when it
39:14
worked, right? Um yeah, if the function fails or it's
39:19
completely synchronous serve zero. Uh and I assume um I don't actually know
39:24
but uh I assume when it runs out of bytes it will return false. Uh let's
39:30
see. [Music] Um
39:35
I don't I don't actually know. I assume it will return false when it can't read all the bytes that you asked for. Um but
39:41
I don't actually know. But what I do know um is we could check for bytes read. So either way uh so what we
39:46
basically want to do is is say okay if it read properly note uh this is uh
39:52
there's still input um or here uh we've
39:58
hit the end of the stream go back to the beginning. So that that was just
40:05
fantastic. I mean that is you know high quality craftsmanship right there. Don't let anyone tell you otherwise. Um so I'm
40:11
going to go down here and try that again. So we hit the L key for looping. We come down here. We hit that. Uh I'm
40:16
go walk back down here and then I hit L. Hey, look. We just wrote input recording
40:23
in 10 minutes. So, uh I mean, all right.
40:28
I will point out that there was an entire company devoted to doing nothing but input recording at one point. They failed. It was called replay something.
40:35
I don't remember what it was. Um, but basically if you architect your code right, input recording and playback is
40:41
literally like 10 minutes or 20 minutes of work. You just watched me do it while explaining it actually and it took only
40:47
that amount of time. So it takes even less if you're not trying to do that stuff. Totally trivial. So the only
40:52
question we have left is how do we loop the state of the game, right? We got to
40:58
loop the state of the game as well because there's all this state that was set up at the beginning and we need that to be true. Well, hey, guess what? Um,
41:06
yes, this is going to mean our files end up being particularly big. That is true, right? Um, if you look in the data
41:12
directory, our food.hdm ourhmi for the input recording is pretty tiny right now, right? But if we want to if we want
41:19
to just say, well, we want the whole game state saved. Well, guess what people? Um, I again architected this
41:26
because I had a bunch of stuff that I knew would be good when we got there, and this is one of them. We virtual
41:31
alloc entire memory for the game in one block that has a known base address.
41:38
Right? So if we want to restore the entire state of the game to exactly what it was at any point in time, all we need
41:45
to do is save or load that block. That's it. It's like two lines of code. No
41:53
serialization layer. No big long walking change of things
41:59
trying to figure out what's the state and what's not the state. No, nothing. We know that if the game saw it, it was
42:04
in here. And we also know that even if the game put pointers in there, they'll
42:09
still be valid because we load it always at the same base address. So we don't even have to care. It doesn't matter.
42:15
The game could do whatever it wants. It does not matter what the game does. As long as the game does it in here, it'll
42:21
work, right? So all I have to do now is when we do um this this game memory
42:28
structure now just has to be part uh of of this guy right of win32 state so that
42:33
we can save and load it. Uh so basically if if I just go in here and and do something like this so the game memor is
42:39
in there now right um and then I say that uh that the game memory um is
42:44
actually something off the wind32 state like this right and and I could actually maybe I'll do it this way just for a
42:50
little bit. uh game memory. Okay.
42:58
All right. Uh so basically what I'll do is I'll go here through here and just change this uh to a pointer. Again,
43:03
needless busy work because C actually did the dot and pointer syntax
43:08
annoyingly, right? Uh so now that should be uh set up in there uh properly and
43:15
there shouldn't be any uh difference. So if I run this, uh, I should just get the same old game working the same way that
43:21
it was before. No, no problems, right? Just what we would expect. Uh, and then what I can do is I can go, all right, so
43:28
now when we when we begin or end recording, right? Um, basically now what
43:34
I know because of the way that I did this thing, I'm going to put that total size member uh in here as well. In fact,
43:40
I guess to be fair, I really didn't need to do the thing I just did with the game memory. Um, that was probably premature.
43:48
Let's let's back that up for a minute. Let's just back that up for one second because I realized that's actually I
43:53
don't even really need to do that. That was just a wasted code edit. That was just dumb, right? Um, I don't need to do
43:58
that at all. All I actually needed to do was make it so this one virtual alloc uh
44:03
was actually trapped. So, basically uh this voidar game memory block, right? um
44:09
this thing that just needs to do that right um I
44:15
just need to remember where that was so basically what'll happen is these will be off the wind32 state now uh the game
44:21
memory block will be off the wind32 game memory block like that uh and and same
44:28
thing here wind32 state total size uh so those two things will work that way now
44:34
right wind32 state total size uh and that uh now gives us what we need in our
44:41
win32 state structure uh to basically go up here and do exactly what I wanted to
44:46
do uh which is when we begin input playback and begin recording it will now actually just restore the state. So uh
44:53
when I do the begin recording input when we open this file handle I am immediately going to just do a full-on
44:59
blast write this thing here write file like that. I'm just going to go ahead and do a full-on blast write of the
45:07
entire game state. Just everything that there is in one call. One simple basic
45:13
call. Now, there is one caveat to it, which is because Windows doesn't actually allow 64-bit writes. If total
45:20
size is actually greater than a 32-bit, so if it's more than 4 gigs, this won't work. We'll have to write a for loop
45:26
that, you know, loops over it. So, I'm going to put an assertion in here just so if we ever get to that point,
45:31
remember to come fix this uh and put a for loop in there. But that's just because Windows doesn't do that for us.
45:37
So, we're going to assert um that total size is basically less than um this
45:43
value right here. Uh just because we know we can't actually pass that FF ff.
45:49
We know we can't pass a value that that's that big. Um so, I'm going to uh assert that that's true. I'm going to
45:55
write out uh basically that entire size here, right? I'm going to have another one of these dummy bytes writtens things
46:02
that we don't care about because again this is total debug code. No, uh user will ever uh touch this code or if they
46:08
do they're they're on their own. Uh uh and then I will just pass uh the
46:15
memory, right? Game memory block. There we go. Um
46:21
so that's it. uh and the opposite is is exactly true as well. So we'll just go
46:26
in here and uh do the read file. Uh so we basically have when we begin the input playback when it does that file
46:32
handle uh we're going to again do the assertion but then we're just going to do a read file uh on the playback handle
46:38
and the game memory block total size stuff is all the same I believe and bytes red is here as well. I think
46:44
that's all uh the same conversion to dword. Uh, right. Well,
46:51
actually, I guess bytes to read equals this is probably the serer way to do
46:57
this. We just cast it down and then verify uh that they are equal after the cast, right? Uh, so bytes to read, there
47:07
we go. Uh, and same with bytes to write. Bytes to write, we'll go ahead and grab
47:13
that total size there. keyword and um
47:20
assert that they are equal as well. Right to write.
47:27
All right. So, let's see if that does anything. All right. So, my guy is down here. I'm
47:32
going to start him in the corner. I'm going to hit the L key to start recording. Uh and let's see.
47:39
I do not know uh how long that's going to take. There's a bit of a pause there when you do it. This is a very slow hard
47:44
drive as you might imagine. And hey, look,
47:52
that was it. Uh, of course, we didn't actually loop properly, so that's not
47:57
good. So, we still have a bug. Why didn't we loop properly? Um, does anyone know? Does anyone already know why we
48:04
didn't loop properly? Because we should have looped properly, but I don't actually think we did. It stopped when
48:10
it got to the end, which it should not have. Uh, so I guess I still don't
48:15
actually know if this ever happens. Uh, because it may be that it just returns zero for bytes red. So I'm going to go
48:21
into that playback input. And that's the that's the only thing that didn't work. Um, so I'm just going to go in there and
48:27
really quickly check it. Uh, just to see what's going on there. I I don't I'm so bad at Visual Studio. I never know how
48:33
to just jump to a function. You know what? I'll just do it this way. Uh, wind32 playback
48:39
input. There we go.
48:50
So, uh, all right. Win32 playback input. I go in here and I want to know, do you
48:55
ever get there? Basically, that's what I want to know. L
49:03
no. So, it's as I suspected it. I think it even if it doesn't read any bytes
49:08
because it hits the end of the file, I think it just returns zero in bytes read. Uh so I'm going to go ahead and
49:13
say that uh what we really needed to check was not that right. We do the read
49:18
file here. Um but what we need to check instead is if bytes read equals zero uh
49:24
then we try to do this. Now we could do that only if the read file succeeds. So we if the file handle like isn't valid,
49:31
we we don't do it. So that might be better. Uh so basically when we get to the end of the stream if the bytes read
49:37
uh is is zero we've we've hit the end of the file and we'll restart it. So let me just see if that was the problem that we
49:42
were having. Um so L and then I will move us down to the
49:50
corner and then L again.
49:57
And we'll see if we've got our loop edit working now. Yeah. So basically that's it. We now
50:05
have looped editing just working. Um I mean it's kind of when you actually
50:11
see how simple it was to implement it's kind of like oh duh like how come we're
50:17
not always doing that because now you can literally do anything that you want. Like anything that invol that whatever
50:22
you wanted to do to tune that that jump code from before I can just do right. um because I can just go ahead and say, of
50:29
course, I didn't actually I don't think I actually have a way to turn off the playback. I got to go actually add some real controls to this. Uh but basically
50:35
now if I want to tune that jump, um I can do that in a much more interesting way, right? Um I can go ahead and say,
50:40
"All right, let me set this up. Uh let me start the recording, right? I'll go ahead and and do whatever the the motion
50:47
is that I want to do. So I'll go in here. I'll do the jump. Uh and then I'll finish it and I'll loop that. Right? So
50:53
now it's looping uh looping my jump. And I can kind of go ahead and say, "All
50:58
right, so now let's let's go ahead and edit the code uh for that jump until I'm satisfied uh with how it works." So I go
51:05
in here and I say, "All right, where was that code for the jump? Here's the code for the jump, right? First of all, it
51:11
should go up and down again." Uh so basically the the sine wave should at least go through a pi period, right? I
51:17
would think. Um so basically we want it to be like uh I think I have a pi 32 defined, do I not? Um, so I'll recompile
51:23
that, right? Uh, and I think that should change the period of my sine wave in
51:29
theory. No, that did nothing. Okay, this is actually working. Is it not? Like,
51:35
let's say I want to change the height of the jump. Let's find out if that works properly. There we go. So, I've got a
51:40
smaller jump. So, I just didn't do this right. So, the T jump, uh, if it was doing pi 32, that should be one thing
51:47
round. It should go up and back down again, shouldn't it? Unless I'm mistaken. Uh, so the t starts at one, it
51:54
gets subtracted down to zero. In theory, that should do it. Well, let's let's see
51:59
what happens. Who knows? This is the whole point of this, I suppose, at some level. Uh oh, I know why. Because it's
52:04
only adding, right? That's it's it's only adding the actual Right. Right. Uh
52:10
so you we actually do need it to go through uh the full the full state of the two pi. Oh man, this is the worst
52:17
jump code that anyone's ever written in the history of mankind. I'm I'm embarrassed to even It's not even jump
52:24
code. I don't even know what you want to call it. Uh but there at least now I've got him uh doing the jump. So that's
52:30
good. Uh and maybe we will also say uh okay, let's actually make this jump go
52:36
like that. And I'll recompile that and see how that's going. So there we go. There's that jump again. A little more
52:42
reasonable. Maybe I want the jump to take a little bit longer. Uh, so if the T jump was more like four, maybe. I
52:51
don't know if that's actually going to do anything for me, but I'll try it. There we go. There's a longer jump,
52:57
right? Um,
53:03
I guess at this point, since I basically finished everything I want to finish, I should probably try to actually write some jump code that isn't absolutely
53:09
awful. Um, I don't know if that's a good idea or not or if we should just go back to what we're doing. I don't really want
53:15
to talk about jump code at all. So, maybe the best thing to do is say, don't ever do anything like this. This is the
53:21
worst. This code is absolutely horrifically bad. You would never write a jump code that even involved any of
53:27
the things that I just did. Uh, it was literally just what came out of my hands when I decided to start typing. So,
53:34
let's we'll wait. We'll actually talk about how to implement these things correctly later. Um, but you really,
53:39
really, really don't ever want to do what I just typed in. Please. Uh, this is horrible and awful. Uh, like we need
53:45
a velocity and there's all this it just don't please don't do this. I'm sorry that I even brought it up. Uh, it's it's
53:51
awful. But anyway, point being, we now have exactly what I wanted to have, which is the ability to do looped edits
53:58
uh on our game. And uh, really the only problem here is performance. Uh
54:04
basically if we go look at um if we go look at how big that file is, you can
54:09
see there's this giant honken one gigabyte file, right? Uh so basically doing that memory blast can be uh slow.
54:16
Uh this is a machine that has no SSD drive or anything like that. So actually I suspect uh on a modern machine this
54:23
would actually be almost instantaneous though. Uh because if you had a fast if you had like a SATA, you know, three or
54:28
whatever, I don't even know what they're up to these days. the SATA 3 SSD drive. I think this would just be instant. Um, but this basically allows you to do all
54:36
of your code tuning with a sequence that gets recorded and it keeps resetting the entire state of the game properly at the
54:42
beginning. So, you don't even have to pick up the controller. You don't even have to take your hands off the keyboard, right? Um, and there's other
54:50
things we can do that'll make this even cooler. We'll continue on this tomorrow. Uh, because basically there's still a
54:56
little bit more we can do that's interesting. Um, for example, one thing that would be totally crazy, um, that we
55:02
could probably do, and I'll just throw this in there for for for kicks, just to give you a little bit of a of a of a
55:07
heads up, because we're basically done with our state recording stuff. I mean, that's that's everything we needed. Um,
55:13
what we can do as well is inside uh when we actually create our window, right?
55:19
When we create our window ex, uh, I believe we can also do some really crazy stuff, right? uh which is we could
55:26
create like um a topmost window. So if
55:31
we do like I I'll just show you what I mean real quick. So if we create window ex right when we when we pass that we
55:39
have this ability to specify these extended styles and there's some of them that are like you know uh that this is a
55:44
topmost window I believe is the thing you can do. Um so this kind of a thing.
55:49
Uh, so we can pass a topmost window and we can pass a um a layered w there's a
55:55
like a layered windowed flag. Uh, I don't actually know where that is. Um,
56:00
context control parent. I go for layered layered. Uh, I think this is this is the
56:06
one we want. Uh, yeah. So, we can do WSX layered. Of course, we can't do that with own DC uh or class DC which I think
56:13
we were using, right? Were we using those? Let's take a look. Own DC.
56:19
Uh yeah, we were using own DC. So we may want to remove that. Um yeah, excuse
56:26
forever because we're not sharing with anyone. So that may be something that we don't actually want to do anymore because of that. Uh so yeah, I'll I'll
56:33
throw this in there. But so if we do this, right, if we actually create a topmost window, uh that's a layered window or maybe I I don't know if I'll
56:39
specify that quite yet. Um but basically if we create a topmost window at the very least right um then I believe uh we
56:48
can basically have the game window right uh can sit on top of our editor as well um so it can kind of be a little more
56:55
unobtrusive right so we can come in here uh we can set the recording marker uh we
57:00
can go ahead and do you know that stupid jump that I was doing or whatever uh like that stop it um have that looping
57:07
and we can just be playing that essentially in here while we're doing our edits full screen, you know, like
57:12
not having to do anything. So, we can basically tune my absolutely ridiculously terrible jump code. Uh, that is awful. Uh, right. I can go ahead
57:19
and do that. And there, you know, we've we've done whatever it is that we wanted to do, right? Um, yeah. So, you get the
57:26
idea. And then the other thing that we can do, and the thing that I was going to suggest is we can also change this into we can tell Windows that we want
57:32
this thing uh this window to be translucent potentially. And of course, we can't do that with a with a CSDC
57:38
window. So we don't we don't really need that CSD stuff. Uh so what we'll do is
57:44
we'll actually just go ahead and the time that we need the device context we'll just get it right because there's only one time I think that we actually
57:51
need it. Uh which is here. Uh so if I just go ahead and do that here and do you know actually get it the way that
57:56
you normally get it. I think we should be fine in theory. So if I do that I believe that we'll be fine. I don't
58:03
actually know. Uh but let's see. Uh well that didn't work so well. Now our
58:09
window doesn't show up at all. I think we may have to do uh this there's like a layered window update uh call. So what
58:16
is it? It's layered window wind32 update.
58:23
Uh, I think there should be a way uh to just say
58:30
um
58:35
um make use of more pop set layered window attributes.
58:41
I don't know like we're we'll have to deal with this tomorrow because I probably won't be able to get it working just now. Uh so I may back this change
58:48
out real quick before we go. I just wanted to see if I could slam it in there. Um but basically you can make a call that basically says please make my
58:54
window translucent, right? Um and so what I could what I was going to say is
58:59
well we can just make uh once after we create our window uh we could also go ahead and make that window be
59:06
translucent, right? We could basically say okay hey windows uh please make make this um this window translucent. So
59:13
let's see uh color ref transparency color all pixels uh color ref. We don't
59:19
actually want one of those. Uh so we want LWA alpha. So we basically want to
59:25
just tell it that our window has sort of an opacity level, right? And that's what we can do by specifying LWA alpha. We
59:31
can specify an alpha level. It's from 0 to 255, I believe, is is the thing. Uh
59:36
the color key uh value, I don't actually know what they want us to do for that. RGB macro. So this is just Windows
59:44
nonsense here. Um, so basically I think I don't know if this is actually true or not, but I think we may be able to do
59:51
something basically as simple as that. Uh, where we just can have a layered window that's just transparent. Um,
59:57
yeah. So there you go. So that's a transparent window, right? Um, so you
1:00:02
can imagine, right, us having the game running and we can edit the code and we can even have a window uh that's
1:00:08
transparent. Did I not fix all the bugs here? thought we fixed this, but apparently we
1:00:14
didn't. Um, break. Of course, our transparent window
1:00:19
is we need to make it not clickable. So, there's a bunch of things we'll have to do to actually make that go. But, yeah.
1:00:25
So, uh, what did I mess up here? Did I mess up the computation of of end of buffer bytes per pixel because we're
1:00:31
writing we're still writing out of bounds? Um, which is not Oh, fantastic.
1:00:37
Fantastic. Um, let's take a look here.
1:00:44
That was not the way that code should have been written. Copy pasta, as people say, although that
1:00:50
wasn't copyp pasta. That was just a a quick edit. So, basically what I had wanted to do in that code that renders
1:00:56
the player is I'd wanted to basically just say if the pixel pointer is still within the buffer, then it's okay to
1:01:02
write it. Otherwise, it's not. And so, I was checking it here. So, this pitch pointer, I don't know why that that's
1:01:07
even in there. It doesn't have to be in there, but basically so this is not actually correct at the moment. Um, so
1:01:14
this is less than end of buffer. I guess this is writing to a this is technically uh has to be the pixel uh plus 4 has to
1:01:23
be less than or equal to the buffer. I don't know if that actually is relevant um but it could be off by that much. Is
1:01:29
there anything else I'm missing in there? So if the pixel pointer if it's greater than the memory pointer uh and less than so buffer memory plus the
1:01:36
bytes per pixel times the width uh the pitch oh that's not right. Yeah that's
1:01:41
just wrong as well. I don't know why I added that in there. So this is basically this is the entire size of the
1:01:46
buffer. It's the pitch which is how many to advance per row times the number of rows. Uh so that's actually correct. So
1:01:52
that's that's more that's more what we should have had. Yeah. Uh so all I was
1:01:58
going to say is if we do that thing with the layered window uh update
1:02:03
layered window uh or sorry set layered window attributes this set layered window attributes thing we can actually
1:02:08
stuff uh in our app in our uh our window handle thing or our thing that handles
1:02:14
window messages. And this WM activate app thing is something that Windows will call us back on depending on whether or
1:02:21
not our window is active. So basically we could make a thing where we said all right uh when our app is active right so
1:02:29
if the app is active uh the w pam that comes in with that message says whether
1:02:35
or not the window is activated or deactivated so if it's true we're going to be activated if it's false we're
1:02:40
going to be deactivated and so this is like activating and deactivating a window right bringing it to the foreground or moving it away and so I
1:02:47
believe what we could do is just say like if the the w pam m equals true. Uh
1:02:54
then we want to actually set ourselves to full alpha. Otherwise, we want to set ourselves uh to to dim alpha, right? So,
1:03:00
we could do something like this. And again, I'm just kind of slamming this in here because we're out of time on the stream. So, I don't want to go any
1:03:05
further on this, but I just wanted to show you kind of what I'd been thinking about we could do. So, we do this stuff.
1:03:10
We're doing some things. We play the game. We do something, right? We're like, ah, this isn't working right. So, we go, let's set a loop point. Get to
1:03:17
the state we care about. Set the loop point. We go through. we do the action that we wanted to have happen. Again,
1:03:23
hopefully that's much quicker on any modern machine. And now we're in looping mode. We can then alt tab to our editor.
1:03:29
Uh, and the game still runs behind the editor uh totally translucently. So, we can still edit full screen and see all
1:03:35
our code pretty easily. Uh, but we can see what the game is doing in the background anyway. And then when we want
1:03:40
to see it um again, we should be able to switch back to it and then it restores
1:03:46
itself to full color. Right? So, a lot of work we still have to do to clean this stuff up, but that was the end of
1:03:52
the the sort of excursion into crazy Windows tricks thing. Again, uh
1:03:59
I know it it hopefully that seems both very very cool because I've literally
1:04:04
never seen anyone demo anything that had this feature before. Um in fact, I'd even seen things like Brett Victor had
1:04:10
something that people were sending around where he was like, "You should be able to just have the code doing what it does while you edit it or something." I
1:04:16
don't know. Um, and I'm like, well, you can do most of that already. Like, we can just do it. We just did did it. We
1:04:21
we just showed how to do it, right? And you could imagine extending this technique very easily as well. Like, let's say we wanted to see the path that
1:04:28
the guy took. Well, all we'd really have to do is start annotating our game a little bit to write to a buffer that
1:04:34
shows that uh those things and draw them over time. And then you've basically got the ability to type live code and see
1:04:41
like paths and stuff update as you are changing the live code. Um, so I think
1:04:46
like yeah, hopefully you hopefully this uh did look particularly cool. I think
1:04:51
it will look really really cool once we're actually working on the game and can just tweak stuff really tightly without ever having to have anything. We
1:04:57
don't have to have a scripting language. We don't have to have any of that. None of that work just works. Uh, but also so you can see that if you architect stuff
1:05:03
properly, it goes from being something that you can't even conceive of spending the time to do to being something that
1:05:09
literally took an hour with explanation. Right? I mean, we architected our game
1:05:15
uh platform layer boundary properly, and now this was free. Like, it was literally someone could do it on their
1:05:21
lunch break and have something that's much more powerful than the editing you actually get in a lot of tools that are
1:05:27
supposed to be very powerful because they have scripting languages or all this other stuff. But actually, a lot of them don't even do this, right? Um, a
1:05:35
lot of engines that you buy don't even have this feature built in. Uh, and we already have this feature from day one, and it only took us an hour. So,
1:05:42
basically, uh, that is the end of what I wanted to cover on this stream, and that's the end of our sort of fun with Win32 stuff. Tomorrow and Friday, we
1:05:50
will be on, um, we'll be at 11:00 a.m. We'll be on holiday hours. And basically what we'll be doing on those two days is
1:05:56
cleanup. So, we'll basically be going through our code, going through all of the like notes I have in Win32 things that need to be cleaned up. Uh, we'll
1:06:02
make this uh, a little cleaner how you interface with the looping system. we'll we'll you know we'll we'll round off the
1:06:08
edges and and make sure everything's set for going into Monday when we start writing uh our actual game engine. So I
1:06:14
hope you have enjoyed the past three days. I know I have. I was really psyched to get this working. I was like this is going to be so much fun and it
1:06:20
was uh so I'm pretty happy with how this turned out. And um let's let's go ahead and go uh to the Q&A.
1:06:30
Please put siratory in your question uh so I can see it. And um please stick to
1:06:37
questions about stuff we've already done either today or on a previous stream. Uh let's not talk about future stuff
1:06:42
because we're going to get there uh eventually. Uh and and off we go.
1:06:51
Okay. Does win32 playback input need to return status in case the file read fail
1:06:56
the file read fails and or reaches the end since in case those cases the function output data structures will be stale or invalid. Uh it's up to you
1:07:04
basically it depends on I mean probably you would want to do that eventually
1:07:13
just for why not sake. Uh but this is only for our internal use. So if it
1:07:19
can't write to the file for some reason, you're going to know because things are going to kind of mess up immediately. Uh
1:07:24
but yeah, I mean when we have more of a logging system and stuff like that, it'd be it'll be easier for us to just note
1:07:30
those errors. Um because really the only thing you're going to do at that point is note the error. There's nothing you
1:07:36
can do about it. Um you know, you you're the user was trying to play back some
1:07:41
input and they can't. And so they really what they need to know is that something failed so they can go see why their hard drive is messed up or whatever. Um, so
1:07:48
yeah, so I do have to remember to delete this file so that I don't end up shipping uh the HMI because we need to
1:07:54
move the other thing that we need to do is we need to move that um that file into the build directory. So that's know
1:08:01
we know that it's a temp file. So I want to make sure we do that as well. In fact, I'll put a to-do in there uh so that we don't forget to do that. So,
1:08:08
win32 record input uh let's say
1:08:14
to-do these files must go in a temporary build directory.
1:08:22
That's very important. We'll write that down in the notebook as well. Path for HMI files.
1:08:32
Okay. All right. Will this break when you make changes to the game game state structure? So yeah,
1:08:40
uh basically so if we wanted to do the full version of this, we would need to have code
1:08:48
annotation so that we know what our structures are and we can backport them when we load. That is just a lot of
1:08:54
extra work um to do. It's it's even a
1:09:00
lot of extra work to do even in a language that has better introspection features than C. Uh C is awful at
1:09:05
introspection. It's one of the places that it really sucks. Um and so uh like
1:09:11
that is one place that like I think other languages do a much better job is because they support that sort of thing.
1:09:16
And it's it's absolutely awful that they've never added that stuff properly uh in so long. But that that's a big
1:09:23
problem. So I don't want to actually go down that route because of the amount of time it will take. And all it means is
1:09:30
that we will have to just restart the app a little more frequently, which just doesn't seem like a bad trade-off for
1:09:35
this project. But if you were doing this in the industrial strength sense where you wanted to make like a game engine
1:09:40
that had this feature and lots of people are going to use it, you would go ahead and do the annotation. Uh, and that way when you make changes to your strus, it
1:09:47
will still work. Um, there's still things that won't work. I mean, this feature is for tuning. It's not for
1:09:52
doing all of your development inside of a loop, right? uh because if you do that uh you can't change the way in which
1:09:59
things get initialized either like the super structures of things. Uh you might change how that how things are organized
1:10:06
entirely and nothing's ever going to make that work live. Um because it you'd
1:10:12
have to write specialized functions to do the mutation and the person would spend more time writing those functions than they saved by getting the tuning.
1:10:18
So you want to focus on just the tuning here. But even with that, we probably
1:10:23
will be able to do some stuff where we just leave padding at the ends of our strcts while we're developing and we can add things pretty easily. We just can't
1:10:31
rearrange them. It wouldn't be too hard to set up the
1:10:37
render ghost or to use two windows to play loops of current code in previous version, would it? Would be cool to see how game states uh diverge. Yeah. Well,
1:10:45
so we've set ourselves up perfectly for doing what you just said because remember the other thing we can do is we
1:10:52
can load multiple game DLS and play them separately. So we could actually load
1:10:57
two game DLS and be playing them back at the same time if we wanted to, right? We could totally do that and it would be
1:11:03
absolutely no problem whatsoever. Um, so like where we do that load library,
1:11:09
right? this thing here, uh, this win3 low game code, we could call that with
1:11:14
multiple on multiple DLL versions. So, we could totally look at two game state
1:11:19
sets, uh, separately if you wanted to. I don't have a use case for that right now, but we've totally set ourselves up
1:11:25
to do all that stuff. Um, you could do anything you wanted with that.
1:11:30
Why didn't you use a switch for all the Vcode stuff? Uh, I don't know. I just just didn't. Um,
1:11:38
we could, but I usually I don't like to use switches inside of switches. I find that gets a little confusing, so I
1:11:43
didn't. What we probably should do is just move this guy out into its own function entirely, which is process keyboard or something like that. But
1:11:49
yeah, there's no real reason since it's mostly zeros, do you think
1:11:55
it'd be worth writing out the game state in a sparse way using a simple r or something? Or is performance sufficient?
1:12:01
Or perhaps that would make performance worse? Can you talk a bit about the trade-offs that may exist there? Um, I
1:12:06
think that basically just buy an SSD drive. Buy a fast SSD drive and you will never care. I think you can blast a
1:12:12
gigabyte to an SSD drive in a split second anyway. Um, so I assume that it's not a problem, but I don't really know.
1:12:18
I would test it on a machine that isn't 8 years old or whatever this one is now. No, five years old. I'd test it on a
1:12:25
machine that wasn't 5 years old for us before deciding to do any work there. But yes, we could do tons of stuff. Uh
1:12:30
you could also just for the time being uh I think we're what are we we're allocating a gigabyte just don't
1:12:35
allocate a gigabyte allocate um uh much less but there's another thing that we can do too um so one thing that's worth
1:12:43
noting is I split this up into permanent and transient storage and we haven't really talked about what that entails
1:12:49
what we will probably be able to do in the future if we really want to is actually uh just loop on the permanent
1:12:55
storage and the transient storage will reconstruct itself because that's how we'll be architecting the
1:13:00
But there's trade-offs to that as well. Uh so you'll you'll see when we get a little further on other things you could
1:13:07
do there. But I suspect that if you just buy a fast SSD drive, this is just not an issue. So you should
1:13:13
just because this is strictly code for development, you shouldn't spend a lot of time trying to figure out how to make
1:13:18
it better because I think it's probably sufficient and just buy some hardware that works well. What's on the schedule for next week? Uh
1:13:25
next week is basically going to be a educational sort of overview week. It's going to be me talking about game engine
1:13:31
architectures. What's what is in a game engine? What are the parts of it? What are the parts that we have to build and
1:13:36
why? How should they be sort of uh looked at and understanding them. So we
1:13:42
probably won't do very much actual code next week. Uh we may try to like load a bit map and put it on the screen or
1:13:47
something, right? Uh, but we're not going to get much further than that next week because there's going to be a lot of sort of educational materials that
1:13:54
kind of have to we have to go through. Are you going to show how to create more
1:14:00
debug tools like this? Uh, yes. There'll be at least one other major debug tool
1:14:08
too. There will be at least two more major debug tools that will get created. Um,
1:14:14
but I I wanted to do this one for fun because I felt like we needed to do
1:14:20
something fun because we had done a bunch of hard work on Win32 that was basically just about managing Win32 and and all the stuff it makes us do. So, I
1:14:26
want to do something fun with Win32. But one thing that you probably will notice is that we could probably make a bunch
1:14:32
of this uh stuff actually live one layer above the platform layer because most of
1:14:37
the stuff that we just did wasn't really platform specific. So I don't want to write too many debug tools in the
1:14:43
platform code because we can probably actually leverage them across multiple platforms. Um so basically uh yeah we
1:14:50
won't be writing any more debug tools until we've got more of a superructure in place on the game side of things. Uh
1:14:57
but then we'll we'll we'll there'll be at least two more debug tools that we'll go over
1:15:02
regarding saving game state for recording. Won't we run into problems if we port to a system that doesn't allow
1:15:07
us to specify a base address? Um, well,
1:15:14
I'm not sure what you mean because basically we can just always develop on
1:15:20
whatever platform we want to, right? So our main criteria for what is the
1:15:26
platform that we want to develop on should be ability to specify a base address,
1:15:31
good debugger, fast compilation tools, um stable non-crashing system, good
1:15:38
graphics drivers, right? Those are like the things we want out of a dev. So whatever we choose as our dev platform,
1:15:43
that's what it should be. When we port to other platforms, we're not going to be tuning like we're not going to tune the game code on some other platform.
1:15:50
we're just going to do working on the port, right? So, we shouldn't really even need game state recording on other
1:15:56
platforms. Uh, but the answer is yes. If you are on a platform that can't specify a base address, then this gets
1:16:01
infinitely harder. Uh, and but we don't ever need to do that. So, we don't really have to worry.
1:16:10
Any chance for highle overview while cleaning up? Yes. Uh, there will be a bunch of that Thursday and Friday. I'll
1:16:15
do some recap stuff as well. How often do you look up code or study code like you have done today?
1:16:23
Uh, how often do you look up code or study code like you have done today? I didn't
1:16:31
did I study code today? I don't know. I'm not sure what that means. Sorry, I don't understand the
1:16:36
question, but if you want to um more like elaborate on that question a bit, I
1:16:42
could do it. Um, isn't a two gig snapshot of your game
1:16:47
memory crazy huge, though? Is a two gigabyte snapshot huge?
1:16:54
Well, let's answer that question, shall we?
1:17:00
Newegg.com. Let's see just how huge that is. Uh,
1:17:06
let's see here. Computer hard drives, SSDs. Let's see. If we wanted to put an SSD in
1:17:13
our system and we wanted to spend a reasonable amount of money, let's say $100 to $200, how much would it have on
1:17:21
it? The answer is it would have, let's see, $130 seems pretty good. 240 GB. So,
1:17:28
you could store 100 game states,
1:17:33
a hundred of them on your SSD drive.
1:17:39
So crazy huge. I don't know. How many would we even be storing? Four, five.
1:17:45
We're talking about a number so tiny that the hard drive laughs at you that you think that it's crazy huge, right?
1:17:50
Like they they don't care at all. And these are dirt cheap, right? Um like you
1:17:55
know, and they're getting bigger every day. So and and you can look at here. I don't know you can see there yet some
1:18:01
random um specs on them. I don't know what their their but their max sequential read and write uh like is is
1:18:08
absolutely huge. So 400 megabytes per second means that it basically take like four or five seconds uh right to do the
1:18:15
right depending on what it was. Um so as long as you're okay with that that's
1:18:20
that's the end of it and it's in the cache the rest of the time. Like the other thing to remember is I did that
1:18:25
because I wanted to do it to dis to disk but we could do it to memory and then it would be largely instantaneous, right?
1:18:31
So, if we wanted to, we could just make this go to memory and then lazily output it to disk, which uh you know, honestly,
1:18:37
now that I say it, might be a thing that's worth doing just so we can have that speed and show how to do it. Maybe someday when we when we're doing a
1:18:43
stream where we just want to do something fun, uh maybe I'll do that. So what I'll do here is I could say for
1:18:49
example to do Casey lazily write uh the giant memory
1:18:57
block and use a memory copy instead. So that way you just have more memory in
1:19:03
your machine and it just works. So you could do that. That would be one that would be one way to go certainly.
1:19:12
Let's see.
1:19:18
I'm definitely Batman. I think your sine wave is a tad bit off on the land of the jump. No, man. That jump code is
1:19:23
awesome. You don't know what you're talking about. That's the That's the awesome jump code, man. That feel, that jump feel.
1:19:30
Uh, let's see.
1:19:38
I'm thinking maybe less about the error and more about what happens at the end of the stream when it loops. Doesn't
1:19:44
that generate an extra repeated input event played back or am I missing something?
1:19:53
I see what you're saying. Um, you may be correct about that. So, the I believe what the person is saying is that when
1:19:59
we do our input playback, uh, we need to possibly also still read the first.
1:20:09
Yeah. So, so I think he is correct. Basically, our
1:20:15
new input strct here will still get passed to the game um because we tried
1:20:20
to read to it, but the read failed. So, it's still whatever the input was. Then we start the looping, but we never try
1:20:27
to read it again. Uh so really what we want to do here is we still want to try to do one read, right? I think is is
1:20:36
what he was saying. And I think that's correct. So what we probably want to do here is basically say all right when we have to do that that loop um when we do
1:20:43
when we begin the input playback I think we want to actually do the read again. So after we read the game state in we
1:20:49
then read the input again. Good catch. Yes, that's totally I think that's totally more correct um that way than it
1:20:56
was before because otherwise before there's a like a leaky frame of input basically that's not correct. Right. Uh,
1:21:03
so let's see. Excuse me. Let's see if that's true.
1:21:14
Yeah. So, it looks good. So, I think Yeah, I think that's more correct. That's a good catch. Thank you for catching that.
1:21:25
Can you clarify if function pointers are part of that game state block? maybe elaborate on what could ever cause it to
1:21:31
fail or misalign. Um, so
1:21:40
that is a good point. Basically, inside here.
1:21:47
If we ever store a pointer to to a function, uh, oh,
1:21:54
wait a second, wait a minute, though. So, I'm about to
1:21:59
say something that was a limitation, but I don't know if it actually was. Let me see something here. I want to I want to
1:22:05
find out if it actually is or not. Um because I'm not entirely sure. Is there a way to load a DLL at a known base
1:22:12
address? Um yeah. So, there may even be something you could do to fix that as well. I have
1:22:19
to think about that because then you'd have to you have to
1:22:25
make sure the linker was padded out. I don't know. All right. Sorry, forget that. Sorry. Sidetrack. Um, so yeah,
1:22:31
what you can't the only thing that you can't do is since the DL gets loaded and unloaded when you make a change, uh,
1:22:39
game state playback will work fine with function pointers, right? So you could have pointers to functions in the game
1:22:44
state. And the game state playback stuff we just did would work absolutely fine. What won't work fine is if you store any
1:22:51
pointers to functions that are not updated whenever you recompile and reload the DL because the the DL may get
1:22:59
loaded in a different base address and the linker may output different layout for the executable. So basically when
1:23:05
you are doing these techniques function pointers are a bad idea like you do not want to have a bunch of function
1:23:11
pointers. Um thankfully we will I can't think of really anywhere we'll be using function pointers. Um but yes, if we do
1:23:18
use function pointers, then that is a thing that would break. Uh and we would have to try to work around that somehow.
1:23:24
Uh probably anywhere we would use function pointers would be relatively easy for us to work around. Um but yeah,
1:23:30
what would be nice is if we could tell the linker to always be incremental and never move functions unless it
1:23:36
absolutely have to. Uh and then if it did move the function, leave a stub there. And also to tell Windows to
1:23:42
always load the library at a known base address that we tell it. um which I don't know if you can do if we could do
1:23:49
those two things this would literally be bulletproof like you could you could just go ahead and do it and so that it's
1:23:54
actually mostly limitation of those tools that will that don't allow us to get 100%. the technique should actually
1:24:00
just work. But unfortunately, I think we can't. The linker just isn't written to do that. If we had written our own
1:24:06
linker, again, the more stuff you write from scratch, the more powerful you become because there is no reason we
1:24:11
couldn't just make that work. Um, the linker could totally support that and it would just work. Uh, but unfortunately,
1:24:17
we don't have that luxury.
1:24:26
How can we make the playback work if we have no base address? Uh, you can't really um, you really need
1:24:34
the base address. If you don't have a base address and for some reason you really still wanted to try and do this,
1:24:41
you would have to make sure that all of your code did not use internal pointers to itself, right? it all would have to
1:24:47
be based off of offsets, but you could do that, right? And in fact, C could
1:24:52
have supported this, but didn't, and it's a real shame that it didn't. There are ways you could have supported this really easily. Uh, but it just makes it
1:24:58
a real hassle in C to work without um that. So, offsets are the way that you
1:25:03
would do that. Instead of pointers, you'd always use offsets from the base and then you'd always just keep around the base pointer and just use offsets
1:25:10
off of it. So, you can actually make all of that work as well. It's just again the languages aren't set up for it. It's
1:25:16
it's one of those sad things that just it's why we need a new C. It's why we need John Blow to make his new language
1:25:22
and for it to be awesome because then we can just fix all these things and then there'd be no stopping us, right? Because all these things are totally
1:25:28
possible on the computer. It's just either the operating system or the language uh not actually allowing the
1:25:34
programmer to do their job. So
1:25:41
wouldn't the address spaces of two game states clash? Um,
1:25:47
so yes and no. I mean, basically the way that I would suggest doing it, like if
1:25:54
you wanted to load two DLs and you wanted them both to use the same base state, what you would have to do is
1:26:01
you'd have to basically map the memory in and out of the location. So you'd map one memory in, run the game for one
1:26:08
frame, map it out, map the other memory into location, map the other one out, but I think you could do it. So I don't
1:26:13
think it's actually a problem. If you want to have the two memory states in in place simultaneously,
1:26:20
you're you're you got problems. Um, but the other thing that I would say is you
1:26:25
could you could actually do it much simpler by just creating a subprocess. So you could create subprocesses each of
1:26:31
which had their own memory space for example and then you could run the two copies as subprocesses of you with using
1:26:38
the same location in memory. So you could do that as well. Uh all of those things would not be very difficult to do. Again, I just don't know why you
1:26:45
would I don't know why I would want to run the two states of the game like that. Um I haven't thought of a good use case for it. Uh but if you if you had a
1:26:52
really good use case for it, we could make that work. That's a thing we could we could make work. Would
1:27:00
a memory mapped file that doesn't commit to disk be faster to read write from? Uh yeah. So the reason I didn't use a
1:27:06
memory map file is because we need to control when it gets written. Uh because we want to change it a bunch and then go
1:27:12
back to the previous state. Uh so
1:27:18
I don't know if Windows allows you to do like a strobing memory map file where you can basically say okay write the
1:27:24
state of the memory map file. make sure the state of the mem file right now is committed. Okay, now don't do that
1:27:30
anymore. I don't know if it can do that. So there's I don't know if memor files can actually support the use case we
1:27:36
need. Um because otherwise yeah, it would be nice because Windows would know stuff about the changes and and know
1:27:41
what would need to be recorded. So that would be nice. Again, if the operating system gave us more direct control over the MMU, um
1:27:49
we would be able to make this technique way better than it is. But you know, we don't have that control. So,
1:27:58
I thought you were going to use the memory snapshot as a game save, but you're using it like a save state in an emulator. Yes, we are basically just
1:28:05
saying, "Hey, this is good for emulators, right? We might as well. If it works for an emulator, it should work
1:28:11
for our actual game." Uh, but yeah, we we can use memory snapshots um as save
1:28:17
games as well, but there may be reasons why we don't want to do that. So, we'll have to think about that when we get to it.
1:28:31
How easy do you think it is to rearrange an existing codebase so that it supports the instant live code editing feature
1:28:36
and what steps would need to be taken to get there? Do you just separate the platform layer and collect all the game memory and allocations in one place? Uh
1:28:43
yeah, I mean basically what you need to do is make sure that the code is architected roughly the way our code is architected now. Um, and
1:28:51
as long as you can do that, you can do this. Uh, but again, you
1:28:57
can't use function pointers. So, uh, there's there's issues there like like
1:29:03
so I don't know if you can even attempt what I'm doing in C++ for example,
1:29:08
because in C++ objects have vtable pointers in them. So, right off the bat, you're dead in the water because those vtable pointers are pointing to the
1:29:14
wrong place. Now, um, so maybe there's a way to fix that. I don't know. I haven't thought about it because I don't use C++'s object systems and stuff like
1:29:21
that. I don't I don't like anything about C++'s vtables, virtual like all that stuff is
1:29:28
really bad in my opinion. I I strongly advise against ever using virtual functions. Um they're almost always bad
1:29:35
for a number of reasons. And so if you're going that if you have a code that's really object-oriented
1:29:41
inheritancy, I don't know how to work around those limitations. I don't know how to fix the vtable pointers. Maybe
1:29:47
there's an easy way to do it, but I don't know what it is. Uh, so you have to be coding in a way that uh doesn't
1:29:54
have pointers to functions baked into your data.
1:30:03
Will there be a Christmas special? Um,
1:30:08
there's a there's a broadcast on Christmas. I don't know if it's special, though. Might just be a normal day.
1:30:18
How will replay work once you have startup logic for the game? How will it be skipped? Will it just work? Uh, it just works because remember we're saving
1:30:25
the memory state. So basically the startup code for the game runs, you play the game for a while. You get up to some
1:30:30
level, something is not tuned right, you hit the L key. That saves the memory state right there. So everything that
1:30:36
happened previously doesn't need to be replayed, right? It just it knows the state. So it just is replaying just that
1:30:42
one part back. So startup logic for the game is not relevant. It's already happened and is captured in the memory
1:30:48
state that we saved.
1:31:05
Okay. So clarification on the earlier question. How often do you look up code like you have done today? Then how often
1:31:10
do you study code? code defined here as simple functions to complex programs on your own, not due to a job. Um,
1:31:19
I guess nowadays I don't really study code at all. I kind of feel like I mostly know how to
1:31:25
code everything that I want to code. And so usually I feel like the next time
1:31:30
I'll start studying code again is when there's a better language than C that I can do a bunch of stuff in that I couldn't do before and I need to start
1:31:36
learning again. Um, but usually I just don't study code that much. what I will study sometimes is like algorithms and
1:31:42
techniques that I don't know. Um so like uh you know if someone posts a thing that's like um
1:31:49
uh you know here's a new way of doing uh
1:31:54
like it's like like binary search or something and I'll go look at that because I don't know like there's tons of algorithms I don't know and tons of
1:32:01
ways of making more optimal stuff that I don't know. So, I'll often look at people's code posts that are like, "Here's a new technique that we
1:32:07
learned." But like code architecture stuff, I kind of don't feel I usually I almost never see stuff on the web where
1:32:13
I think that the code architecture stuff was was stuff that I would necessarily agree with or if it is, then it's stuff that I already knew. So, I don't usually
1:32:19
get any benefit out of that these days. Uh, so I tend to be mostly reading when I study code, I'm when I'm looking at
1:32:25
code, I'm looking at not the code, I'm looking at the algorithm, right? I'm looking at the the the math that the
1:32:31
person did or whatever because I there's a ton of that stuff I don't know and and there always will be. You can't learn it
1:32:37
all. So I learn I learn that stuff pretty often. Um if that makes sense.
1:32:43
But a lot of that just becomes like when you've been programming in C as long as I have, you probably know most of the architecture options that you have,
1:32:49
right? And so there isn't a lot that I can I I rarely if ever nowadays see something that I didn't already know in
1:32:54
terms of code structure. But in terms of algorithms, there's always something new. there's a hundred new algorithms to learn every year, right? That might be
1:33:00
cool and you're never going to learn all of them. So that's that's pretty good. And also you can look at like papers like you can read SIG graph and stuff
1:33:06
and that's again you're not really studying code, you're learning new algorithms and those are usually just new stuff that gets you thinking in new
1:33:12
ways. So
1:33:19
um perhaps rebase image regarding loading
1:33:24
DL at location. Uh yeah, I don't know. Again, it doesn't matter because the linker is going to mess with us anyway.
1:33:30
So, I think I don't know. I think we're basically just dead in the water with function pointers. Uh we need the linker
1:33:35
to help us. Maybe maybe if we turn on incremental it would. I don't know. Or edit and continue. Maybe it would. I don't know.
1:33:42
Uh let's see.
1:33:48
Let's see. Dynamic base no minus fix to linker and it will load at ox40. So, a
1:33:54
null. I don't know if that's true. Um, I'm pretty sure that with address base layout randomization now for security, I
1:34:00
don't think you're actually ever allowed to force your DL to load in a specific location. So, you can check it out. Um,
1:34:07
it's basically this stuff. Um, ASLR, right? Uh, you can read about ASLR. I'm
1:34:14
pretty sure Windows just doesn't let you load DLS or fix locations anymore. But, but I don't know if that's true. Maybe
1:34:20
you can. Maybe there's still ways because there used to be old flags where you could actually specify what the base
1:34:25
address of the DLL was, but I thought that nowadays it was harder to do that.
1:34:31
Maybe not. Um, so I don't know. You can take a look and see if ASLR messes with you or if it doesn't. But either way,
1:34:38
um, and you can turn ASLR off on the whole system if you want to. But either way, I don't think that's the biggest
1:34:43
problem. It's the linker that's the biggest problem. the function pointers are already going to move even if we load the DL at the same base address
1:34:50
when we change stuff. And so I don't think we can really count on function
1:34:55
pointers and I don't think we really need them either.
1:35:14
How long did it take for you to get to a point where you can conceive of a feature and with little effort know exactly what you need to accomplish to
1:35:21
realize that feature? For example, I have conceived of game state recording and playback before. However, I've never been able to get to the point where
1:35:27
everything was set up in a way to make it easy to do. One small example of that is the fixed base address for the memory
1:35:32
block. Without it, it seems impossible or at least way more difficult. Most of programming seems to be having the experience to know how to set yourself
1:35:39
up to make your ideas easy to accomplish. How long did it take you to get to this level of effectiveness? Um,
1:35:45
so I guess what I would say is probably a very long time. Um, probably at least
1:35:51
20 years of programming. Of course, I was very young when I started, so you might count those years a little bit
1:35:57
less because you're not really that as smart and you don't know as a lot of stuff when you're little. Um, so I don't
1:36:02
know about that, but I would say yeah, probably at least 20 years before you get to be the point where you're pretty fast um at that sort of stuff. But maybe
1:36:08
other people will be faster. I don't know. Um, but it took me a long time. As for uh realizing stuff with little
1:36:16
effort, the other thing to remember is this actually is a very easy thing to do what I just did. Um, it's basically
1:36:22
like, oh, I thought about it for a while, you know, and I was like, okay, it's not it's not too bad. And in fact,
1:36:27
I think the first time I did a system like this was actually like 1996 at D6. I did one that just blasted the state of
1:36:33
memory out to something and reloaded it so that we could do physics debugging and you could single step. So what it
1:36:38
did was it just outputed the state uh of the game every time because we had like this physics block that we could blast
1:36:44
out. So like I first did that actually way back I think I was like 19 at the time and had you know a lot less
1:36:50
programming experience. I first did this thing then. Um, and it turns out that
1:36:55
you can do stuff like that, uh, if you have stuff that doesn't have a bunch of pointers in it and all those sorts of other things. And so it was kind of
1:37:01
building on that and like later I was like, "Oh, okay. I could do this as well with other stuff by just having a fixed
1:37:07
base address or something like that." And then pointers would work too and blah blah blah. So it's just kind of like stuff that builds on each other
1:37:12
over time. But this is a actually relatively simple feature. Complex stuff still takes me a long time. I will
1:37:18
sometimes work a month on a code architecture thing because I have an idea that I think something should be
1:37:25
possible and I I have to refine it and refine it and refine it and refine it and I make tons of mistakes. Um so it
1:37:33
depends on how hard the problem is and it depends on how new the territory is for you. Um and so this happens to be a
1:37:39
particularly easy thing because there happens to be a very clean solution if you just do a few simple steps. those
1:37:45
are much easier to arrive at than other things which can be much more complicated and take a lot longer and
1:37:50
have a lot more intricacy. And so we will probably even see some things on this stream later on we're working on
1:37:56
the game where I have to go through several revisions to get it right. Um and so so this happens to be
1:38:02
particularly simple because there's a few simple things you can do architecturally that make it so that it just works. But um
1:38:09
you know that that was that was a lucky that this this I don't want you to get
1:38:14
the sense that I sit down and it goes that smoothly all the time, right? Um
1:38:20
you will see things on the stream where we have to really work at it. Uh, and that's I think I probably I don't know
1:38:27
if there's programmers who get past that that point too, but I suspect I will probably die someday never having gotten
1:38:32
to the point where it doesn't sometimes take me a month to solve a hard architecture problem.
1:38:43
Couldn't you create an offset pointer strct which overloads the uny star operator?
1:38:51
probably not going to try and touch that one. My first question would be how would it know what the base pointer was
1:38:57
at that point. Um I guess you set a global. I don't know. Um you could try
1:39:02
setting a global for specifying a DL base address. Uh
1:39:08
okay an is supposed again. I don't think the DL base address is that important. Like I said I mean it may be possible to
1:39:14
do now even with ASLR. Um, there's maybe a switch that says, "No, really, even though you have ASLR, I really need to
1:39:20
be at the base address." But I'm not sure if you can. I'm not sure if it doesn't just get overridden. But even so, I think it's the linker that would
1:39:26
thrart us.
1:39:35
Uh, pseudonym 73. We we did talk about OS objects, but we haven't talked about rendering subsystem objects yet, like
1:39:40
shader texture handles can't be part of the game state. Perhaps getting ahead of ourselves, but how do you plan to handle this? Uh, well, that is getting ahead,
1:39:47
so wait till we get there. Wear a Santa hat for Christmas next
1:39:53
week. If I can find one, that may very well happen.
1:40:00
Uh, care to reiterate or can you reiterate the pertinent compiler options that allow this to work as it is or
1:40:07
those that will prevent it? I don't know that there are compiler options that prevent this. I think I don't think these are compiler option related
1:40:14
things. uh enemy mouse. I may be wrong about that, but I don't think there's any compiler option things that we need
1:40:19
to uh that need to be set for this to work.
1:40:28
Um
1:40:35
let's see.
1:40:42
Um, have you got some good explanation on how you architect code? Is that in the text you wrote about your work on the
1:40:47
witness? Yes. Uh, if you go to my blog, the witness stuff does have some articles about architecting code, but
1:40:52
we'll be seeing code how we do code architecting on the stream because there's several things in this game that I've never done before. Like I picked
1:40:59
things. It's like I've never written a system that does blah. Um, and so I will be sort of live architecting it. Like
1:41:04
you'll see me s you'll see me go through each step that I would go through to arrive at a good architecture. And so
1:41:10
you'll see me do like a bad version and fix it and do stuff like we'll go through that process where I'm not just
1:41:15
saying I know how to write this and blam it's right or something like that won't be happening. We'll be going through
1:41:21
there'll at least be a couple places if not many places where uh I have to work
1:41:27
through the architecture live rather than knowing what the architecture should be. Crossplatform code I kind of
1:41:32
know what the architecture should be because I've written it so many times. Um, but definitely like when we get to
1:41:38
things pieces of the game, there'll be pieces that I have never written before on any game uh or in any even in a in a
1:41:45
test thing. Uh, and so then you will and I'm doing that intentionally so that you'll be able to see me architect the
1:41:51
code how I write something I don't know how to write because I think that's probably one of the most important things to learn.
1:41:59
Um, let's see.
1:42:06
Uh someone was asking I unfortunately a lot of simple things have been made
1:42:11
complicated by what I was taught by the correct way of programming which was entirely based on a very myopic view of heavy object-orientedness and lack of
1:42:17
solid low-level understanding. I have to spend a lot of time on learning what I was taught just to get to features which should be easy but were made hard uh by
1:42:24
the kind of programming I learned in school. Have you had similar painful unlearning process? Yes. Um basically
1:42:29
like when I was first exposed to C++ um kind of like when people are first exposed to biotoxins uh in um in like uh
1:42:38
95 let's say uh I started trying to program in what were good like I read like Scott Meyer's effective C++ and
1:42:45
like I read like um you know those I don't remember what all those books are
1:42:50
but like the I don't even remember what who the guy's names are uh but the people I had I had subscription to the
1:42:55
C++ report right Um, and basically it meant that I didn't write a single good
1:43:01
piece of code for probably 5 years. Like everything I wrote was awful. Uh, and it's because those techniques don't
1:43:07
work. They're they're bad. Like, uh, you know, you if you have a Scott Myers book on your on your shelf, throw it out.
1:43:13
It's it's complete garbage and every last thing and it's a waste of time. Um, and so I had exactly the same thing. I
1:43:21
went to work for RAD eventually and it was kind of like detox because like Jeff
1:43:26
Roberts uh and John Miles were both not that kind of programmer. Um and Jeff
1:43:31
particularly is a very uh low-level old school programmer and I kind of started thinking why am I doing all these
1:43:37
things? Programming was so much better back when I only programmed in C and then C++ it's all been awful and nothing
1:43:43
works and it's slow and it's ugly and it takes forever to write stuff. What's going on here? Uh, and then I tried just
1:43:49
not programming in C++ anymore. I said, "Let me go back to C and I'll only start using a C++ feature if I find that it
1:43:55
actually use if I can prove that it was actually helping me." And what I found is basically overloaded functions and overloaded operators were the only two
1:44:01
things that I ever wanted again. Occasionally, constructor destructor pairs for doing things like timing
1:44:07
blocks, but that was about it. And so when I was writing Granny, the 3D uh
1:44:12
animation library for RAD, basically that was when I kind of learned how to program again. And so yes, it's a very
1:44:18
painful process. I um I don't know if I wish I could get the time back or if I'm
1:44:23
glad that I went through it. So now I just know to never do those things and that they're actually bad and I don't just have to speculate that they're bad
1:44:29
programming practices. I just know that they actually are. Um and so, you know, yes, I've been there. I totally been
1:44:36
there. Um and it was it was not fun.
1:44:42
All right. Looks like we've gotten to the end of the questions. Okay. Um, let's see here. Uh, I guess it
1:44:52
is the end of the stream. We have reached we have reached the the the end of day 23. Thank you all for joining me.
1:44:58
I had a lot of fun implementing that feature. I thought it was going to be fun. It was fun. Uh, so I hope you have
1:45:03
enjoyed it. If you would like uh to play around with the code at home, you can always uh pre-order on the
1:45:09
handmadehero.org website and you can download the latest source code every night and play around with it uh do
1:45:15
whatever you would like to do, experiment, try new things, and play around with the stuff that we did. And
1:45:21
uh you'll also of course get a copy of the game when it's released and all that other good stuff. Uh if you have to
1:45:27
catch up or ask questions or anything else video related, the news and forums link will go to a bunch of resources
1:45:34
that you can use, including stuff like a coding resource page that has how uh to run Handmade Hero on stuff like Mac or
1:45:40
Linux with people who have done ports. Uh there's if you want to support the video series, there's a Patreon page for it. Um, there's the tweets obviously for
1:45:47
following the series uh schedule and there's also um on the news and forums there's a schedule posted and of course
1:45:52
there's a YouTube video archive that has uh the videos uploaded. So I will be uploading these right after they finish
1:45:57
I upload them so you can always get them right away when I study them. So thank you so much for joining me. Tomorrow will be at 11:00 a.m. and so will Friday
1:46:05
and it'll just be cleanup days uh getting us set for next week. So if you're around tomorrow morning, please
1:46:10
join me for that. If not, I hope to see you uh recorded. Um, and um, yeah, I'm
1:46:16
looking forward to finishing it up uh, our win32 stuff so that we can basically move on to game stuff, which I think is going to be a lot of fun. So, thanks
1:46:22
everyone for joining me and I will hopefully see you tomorrow.

