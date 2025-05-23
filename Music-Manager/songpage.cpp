#include "songpage.h"
#include "ui_songpage.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QFileDialog>
#include <QPixmap>

QString uploadedImagePath;

SongPage::SongPage(QWidget *parent)
    : QDialog(parent), ui(new Ui::SongPage)
{
    ui->setupUi(this);
}

SongPage::~SongPage()
{
    delete ui;
}

QVector<Song> SongPage::songLibrary;

const QVector<Song>& SongPage::getSongLibrary() {
    return songLibrary;
}

void SongPage::loadSongs()
{
    QString filePath = QCoreApplication::applicationDirPath() + "/../../../data/songs.txt";
    QFile file(filePath);

    // Handling case for MacOS (file structure for reading .txt required is different)
    if (!file.exists()) {
#ifdef Q_OS_MAC
        filePath = QCoreApplication::applicationDirPath() + "/../../../../../data/songs.txt";
        file.setFileName(filePath);
#endif
    }

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        QStringList fields = line.split(',');

        if (fields.size() < 4) continue;

        QString title = fields[0];
        QString artist = fields[1];
        QString album = fields[2];
        QString duration = fields[3];
        QString artPath = (fields.size() >=5) ? fields[4] : "";

        songLibrary.append(Song(title, artist, album, duration,artPath));
    }

    file.close();

}

void SongPage::saveSongs()
{
    QString filePath = QCoreApplication::applicationDirPath() + "/../../../data/songs.txt";
    if (!QFile::exists(filePath)) {
#ifdef Q_OS_MAC
        filePath = QCoreApplication::applicationDirPath() + "/../../../../../data/songs.txt";
#endif
    }

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return;

    QTextStream out(&file);
    for (const Song& s : songLibrary)
        out << s.getTitle() << "," << s.getArtist() << "," << s.getAlbum() << "," << s.getDuration() << "," << s.getAlbumArtPath() << "\n";
    file.close();
}

void SongPage::on_pushButton_back_clicked()
{
    this->close();
    if (parentWidget()) { //returns a pointer to the widget that opened the current window (checking to make sure a parent exists)
        parentWidget()->show(); //therefore can be used to go back
    }
}


void SongPage::on_pushButton_displaySongs_clicked()
{
    ui->tableWidget_songs->clear();
    ui->tableWidget_songs->setColumnCount(5);
    ui->tableWidget_songs->setRowCount(songLibrary.size());
ui->tableWidget_songs->setColumnWidth(4, 80);

    //setting column headers
    QStringList headers;
    headers << "Title" << "Artist" << "Album" << "Duration"<<"Artwork";
    ui->tableWidget_songs->setHorizontalHeaderLabels(headers);

    for(int i = 0; i < songLibrary.size(); i++){
        ui->tableWidget_songs->setItem(i, 0, new QTableWidgetItem(songLibrary[i].getTitle()));
        ui->tableWidget_songs->setItem(i, 1, new QTableWidgetItem(songLibrary[i].getArtist()));
        ui->tableWidget_songs->setItem(i, 2, new QTableWidgetItem(songLibrary[i].getAlbum()));
        ui->tableWidget_songs->setItem(i, 3, new QTableWidgetItem(songLibrary[i].getDuration()));
        QString artPath = songLibrary[i].getAlbumArtPath();
        if (!artPath.isEmpty()) {
            QPixmap pixmap(artPath);
            QPixmap scaled = pixmap.scaled(50, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            QLabel* imageLabel = new QLabel();
            imageLabel->setPixmap(scaled);
            imageLabel->setAlignment(Qt::AlignCenter);
            ui->tableWidget_songs->setCellWidget(i, 4, imageLabel);
        } else {
            ui->tableWidget_songs->setItem(i, 4, new QTableWidgetItem("No Art"));
        }
    }
}

void SongPage::on_pushButton_addToSongLibrary_clicked()
{
    QString title = ui->lineEdit_title->text().trimmed();
    QString artist = ui->lineEdit_artist->text().trimmed();
    QString album = ui->lineEdit_album->text().trimmed();
    QString duration = ui->lineEdit_duration->text().trimmed();

    if (title.isEmpty() || artist.isEmpty() || album.isEmpty() || duration.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "All fields are required.");
        return;
    }

    QString filePath = QCoreApplication::applicationDirPath() + "/../../../songs.txt";
    if (!QFile::exists(filePath)) {
#ifdef Q_OS_MAC
        filePath = QCoreApplication::applicationDirPath() + "/../../../../../songs.txt";
#endif
    }

    //check for duplicate song
    for (const Song& s : songLibrary) {
        if (s==Song(title, artist, album, duration)) {
            QMessageBox::warning(this, "Duplicate Song", "Song already exists in the library.");
            return;
        }
    }

    songLibrary.append(Song(title, artist, album, duration, uploadedImagePath));
    QMessageBox::information(this, "Success", "Song added successfully!");

    ui->lineEdit_title->clear();
    ui->lineEdit_artist->clear();
    ui->lineEdit_album->clear();
    ui->lineEdit_duration->clear();
    uploadedImagePath.clear();
    ui->label_albumArtPreview->clear();
}

void SongPage::on_pushButton_uploadArtwork_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Select Album Artwork", "", "Images (*.png, *.jpg *.jpeg");
    if(!filePath.isEmpty()) {
        uploadedImagePath = filePath;
        QPixmap pix(filePath);
        ui->label_albumArtPreview->setPixmap(pix.scaled(100,100, Qt::KeepAspectRatio));
    }
}

