MathMagicka
======

_Basic_ Android equation solver (with OCR capabilities)

(CS241 Honor session final project)

Demo:
======
http://youtu.be/HjDsHZFQpcw

General Idea:
======
1. Take a picture for a simple equation. 
2. The appp solves the equation.
3. Print result

Group members:
======
Daniel Xu   [dlxu2@illinois.edu]

Yijun Zhang [zhang283@illinois.edu]

Referenced code:
======
- Some boiler plate code for menial tasks (taking a picture, setting up tesseract) was borrowed from these sources
	- MakeMachine.net
	- DatumDroid @ http://kurup87.blogspot.pt/

Project Log:
======
Week one: (2014-10-26 ~ 2014-11-1):

- Setup environment.
- Install sdk & ndk.
- Got android phone from cs inventory.
- Detailed journal entry (Daniel)
	- Today I decided to install the Eclipse + Android SDK bundle. I already had eclipse on my machine using the local package manager, but I couldn't figure out how to install only the SDK in a timely fashion. I had not use Eclipse on this machine yet, so I just cut my losses and uninstalled Eclipse and just downloaded the package from Google. After I extracted everything and set up Eclipse again, I went to follow the tutorial here. I got everything up until setting up an AVD (Android virtual device). It said there wasn't any installed system images. A quick Google search yielded some helpful results. Turns out I have to install some system images from the Android SDK Manager perspective. That took a long time to download and install (1.5 hrs) so I'll just leave it here today.

Week two: (2014-11-2 ~ 2014-11-8):

- Followed NDK tutorial
- Followed tutorials and created rough skeleton of code
- Decided project idea

Week three: (2014-11-9 ~ 2014-11-15):

- Found OCR (Optical character recognition) library which works for NDK (https://github.com/rmtheis/tess-two)
- Downloaded the library and attempted to build in eclipse.
- Detailed journal entry (Daniel)
	- Got everything installed, then followed the instructions in the SDK tutorial to make a simple app that took input and opened a new activity when you pressed a button with that text. Turns out android projects are a mess of directories and confusing as crap. I do not think a simple task should be that confusing. Anyways, I did that example and then took a peek at the other examples. I've come to the conclusion that if we get the main functionality done, we can hack it together and make it look pretty later.

Week four: (2014-11-16 ~ 2014-11-22):

- Daniel Xu:
	- Designed user interface for the application.
	- Detailed journal entry:
		- Installed the Android NDK through the official google repo. Downloaded and compiled the tess-two NDK library (which provides our OCR functionality). Took like 20 minutes to compile. The documentation that the author provided was a little outdated and I had to reverse engineer a few outdated commands. After I got that done, I loaded up a sample project that used the tess-two library, and it sort of worked. The image taken had to be really steady and completely aligned with the horizontal axis. The numbers were a little off, which could prove to be a large problem since numerical equations are the base of our project. Will have to look into manually training the Tesseract library (tess-two is pre-trained). Then I will have to reverse engineer the example code to figure out how they got the camera to work and where all the temporary data went, as well as how tess-two is invoked. Will leave it at here for today.
	
- Yijun Zhang:
	- Try to find and download an possible equation solver API (wolframalpha API)
	- Write a simple equation solver library (like solving 12+x=20, y/12=96...), symbolic manipulation.

Week five: (2014-11-23 ~ 2014-11-29):

- Daniel Xu:
	- Reformatted all of the simple solver library code to conform with JNI conventions
	- Linked together all of the libraries we used to create final product
	- Some notes on issues I ran into:
		- builder in project properties had to be manually set to the full path of ndk-build, for some reason the PATH variable wasn't reading correctly
		- Android.mk changed since the tutorials I viewed, should have been 
			include $(BUILD_SHARED_LIBRARY)
		  instead of 
			include $(BUILD SHARED LIBRARY)
		- a great source of headaches and confusion was eclipse not updating the syntax checker (for JNI C code) upon save (like normal). had to actually run the code to see new evaluation... took me a whlie to realize and was really annoying

- Yijun Zhang:
	- Add some simple calculation functionality (like solving 1+1, 1999/12, 123*321...) to simple solver library.
	- Testing

Week six: (2014-11-30 ~ 2014-12-6):

- Testing.
- Recording video.

Future possible improvement:
======
