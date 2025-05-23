#include "mainwindow.h"
#include "user.h"
#include "songpage.h"
#include "playlistmanagement.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    User::loadUsers(); //Users are loaded in at system startup
    SongPage::loadSongs(); // Songs are loaded in at system startup
    PlaylistManagement::loadPlaylists(); // All user playlists are loaded in at system startup
    User::loadLastPlayedSongs(); // Last Played Songs are loaded in at system startup
    User::loadLastPlaylist();


    MainWindow w;
    w.show();

    int terminate = a.exec();

    User::saveUsers(); // Contents of 'UsersList' saved to 'users.txt' when exiting the application
    SongPage::saveSongs(); // Contents of 'songLibrary' saved to 'songs.txt' when exiting the application
    PlaylistManagement::saveAllPlaylists(); // Contents of 'AllPlaylists' saved to each user's respective file in 'playlists.txt' when exiting the application
    User::saveLastPlayedSongs(); // Contents of 'LastPlayedMap' are save to each user's respective 'last_played.txt' file when exiting the application
    User::saveLastPlaylist();

    return terminate;
}
