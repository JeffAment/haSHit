# haSHit
Command line tool to generate MD5 checksums for files, and verify their integrity later.  Great to check for corrupted files so that you can restore them from a backup before the corruption propogates to other backups.  This program has been tested on Linux based machines, but should work on Apple computers also (will test this at a later date).  As far as Windows support goes, I really don't have any idea if this will work on Windows - I'm thinking it won't, but it could possibly work on the "Linux subsystem for Windows."  If I ever get a chance I might test it out to see if it will work as is, but to be honest I never intended to support Windows officially.  Future functionality that I hope to implement can be found in the code in "//todo:" lines.  Right now my top priorities are to clean up a few things, implement a progress bar at the bottom of the shell, and to add a logging/alert function to make this viable to use in a cron job for scheduled integrity checks of important files in cold storage.

# How to use
This program uses the Boost filesystem library, so make sure you have Boost installed and CMake should try to find Boost automatically using find_package().  The usage notes can be found by running the executable with no/invalid arguments, but I will list them here also:

USAGE:</br>
  hashit [options][command] /path/to/files (if no path is specified the current path is used)

OPTIONS:</br>
	 -r, --recursive		(Traverses the directory and all of it's subdirectories recursively)</br>
	-q, --quiet			(Decreases verbosity)</br>
	-o, --overwrite		(Overwrites existing .md5 files when using generate command)</br>

COMMANDS:</br>
	generate	(Generates a .md5 file for each file in the directory/directories)</br>
	check		(Checks files against their stored md5 hashes)

EXAMPLE COMMANDS:</br>
	hashit -rq generate				(Generates .md5 files for every file in the current directory using the -r and -q options)</br>
	hashit check /home/Samantha		(Checks files in Samantha's home directory against their corresponding .md5 files, but does not recurse into subdirectories)

# Contributing
I am willing to entertain contributions in any form from anyone.  See something in the code you think I could have done better?  Feel free to raise an issue!  There is always room for improvement and I would love to hear some unique perspectives on things.  Want to implement/add/suggest something?  Raise an issue and briefly outline your thoughts and if you would like to get your hands dirty and write some code yourself, mention that you are willing to help.  Please do not submit code without first raising an issue!  While I do value all contributions, I would like to hear what you want to do before you work on it so that nobody wastes their time working on something that isn't in line with the needs/vision I have for this project.  I do not have an official style guide, but try to adhere to general best practices and to stay consistent with the existing style and naming conventions, and please never use C-style braces unless they follow a multi-line initializer list!

# Disclaimer
This software is distributed AS IS with no warranty expressed or implied.  It the end user's sole responsibility to read and understand the code in this project before executing it on any machine.  I hereby release myself from any and all liabilities!
