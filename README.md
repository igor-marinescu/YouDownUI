# YouDownUI
Graphical User Interface for youtube-dl (ytdl-org)
![Alt YouDownUI](/documents/img/screenshot1.png)

## What is YouDownUI
YouDownUI helps you easily, using mouse or keyboard-shortcuts, to download videos from online platforms.
It is a graphical User Interface (made in Qt) which calls in background the youtube-dl.exe application.

You don't have to manually start the command line and invoke youtube-dl with desired arguments. 
YouDownUI does it for you.

A very intuitive User Interface allows you to:
  * download a video by specifying the link and desired video format.
  * add links to a queue to be automatically downloaded one after another.
  * edit and store the queue of links permanently. A link is removed from queue once it is successfully downloaded. 
  * query available formats for a video/audio link.
  * update youtube-dl application.
  * generate a log file locally with all activities.

## Installing
YouDownUI doesn't need to be installed.
Copy YouDownUI.exe to a folder, together with youtube-dl.exe application (https://github.com/ytdl-org/youtube-dl).
![Alt YouDownUI](/documents/img/screenshot2.png)

## First Steps
Execute YouDownUI.exe to start it.
The first step it is recomended to set the output path and filters.
![Alt YouDownUI](/documents/img/screenshot3.png)
Press **Settings...** (Alt+S) and specify:
  * Output Directory: directory where downloaded videos are saved.
  * Default Filters: filters which appears as a dropdown option when a new link is added.
  * Preprocess youtube links: parse youtube links and remove all arguments but leave only the video.  
Press **Ok** to save the new settings.

To download a video press **Add...** (Alt+A) and enter the video link and specify desired format.
Press **Ok** to confirm. The link is added to the queue.

**Download** button activates and deactivates the process of downloading links from queue.
Press **Download** to start downloading the first (top) video from queue.
After a video is successfully downloaded, its link is removed from queue and the next link starts automatically to be downloaded.

During the process of downloading videos (**Download** button is activated). The other functions (**List Formats** and **Update**) are disabled.
To activate them, press again **Download** (deactivates downloading) and wait until currently downloaded video finishes.

## Author

Igor Marinescu - Qt UI

## License

See the [LICENSE.md](LICENSE.md) file for details.

## Acknowledgments

ytdl-org / youtube-dl  (https://github.com/ytdl-org/youtube-dl)
