# YouDownUI
Graphical User Interface for downloading online videos using 3rd party command line tools (yt-dlp, youtube-dl, etc.)
![Alt YouDownUI](/documents/img/screenshot1.png)

## Releases
  - 31.10.2023 YouDownUI 1.1 (MD5 Checksum: 4f22a82a779fa507bca989862e912bc7 *YouDownUI.exe)
	- Added possibiliy to specify the downloadeder tool (not hardcoded anymore).
	- Added possibilty to download only Audio.
  - 12.11.2023 YouDownUI 1.2 (MD5 Checksum: 157e6ab3c1354032ca8b115689da47c0 *YouDownUI.exe)
    - Modified Add Audio/Video button functionality
  - 13.03.2025 YouDownUI 1.3 (MD5 Checksum: 7e10313f35b76e133c609a11a7edd9c1 *YouDownUI.exe)
    - Added the option to append the timestamp to the downloaded file (settings).
    - Added the option to set the uploaded time as downloaded file modification time (settings).
  - 22.04.2025 YouDownUI 1.4 
    - Added linux support 
    
`bin` **folder removed, soon Win and Linux releases will be published.**

## What is YouDownUI
YouDownUI helps you easily, using mouse or keyboard-shortcuts, to download videos from online platforms.
It is a graphical User Interface (made in Qt) which calls in background a downloader (yt-dlp, youtube-dl, etc.) tool.

You don't have to manually start the command line and invoke the downloader tool with desired arguments. 
YouDownUI does it for you.

A very intuitive User Interface allows you to:
  * download a video by specifying the link and desired video format.
  * add links to a queue to be automatically downloaded one after another.
  * edit and store the queue of links permanently. A link is removed from queue once it is successfully downloaded. 
  * query available formats for a video/audio link.
  * update the downloader tool application.
  * generate a log file locally with all activities.

## Installing
YouDownUI doesn't need to be installed.
Copy YouDownUI.exe to a folder, specify in Settings the downloader tool to use (must be downloader separatelly).
![Alt YouDownUI](/documents/img/screenshot2.png)

## First Steps
Execute YouDownUI.exe to start it.
The first step it is recomended to set the output path and filters. Press **Settings...** (Alt+S) and specify:
![Alt YouDownUI](/documents/img/screenshot3.png)
  * Output Directory: directory where downloaded videos are saved.
  * Default Filters: filters which appears as a dropdown option when a new link is added.
  * Preprocess youtube links: parse youtube links and remove all arguments but leave only the video.  
  
Press **Ok** to save the new settings.

To download a video press **Add...** (Alt+A) and enter the video link and specify desired format.
Press **Ok** to confirm. The link is added to the queue.

**Download** button activates and deactivates the process of downloading links from queue.
Press **Download** to start downloading the first (top) video from queue.
After a video is successfully downloaded, its link is removed from queue and the next link starts automatically to be downloaded.

During the process of downloading videos (**Download** button is active/checked). The other functions (**List Formats** and **Update**) are disabled.
To activate them, press again **Download** (deactivates downloading) and wait until currently downloaded video finishes.
After this **List Formats** and **Update** become active and can be executed.

## Author

Igor Marinescu - Qt UI

## License

See the [LICENSE.md](LICENSE.md) file for details.

## Acknowledgments

ytdl-org / youtube-dl  (https://github.com/ytdl-org/youtube-dl)
