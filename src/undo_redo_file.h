#ifndef UNDO_REDO_FILE_H
#define UNDO_REDO_FILE_H

#include <QImage>
#include <fstream>
#include <string>

enum class cacheType
{
    action
    , canvas
};

struct UndoRedoFileArgs
{
    QImage * Image = nullptr;
    int Position = 0;

    UndoRedoFileArgs(QImage * image
                     , const int& position)
        : Image(image)
        , Position(position)
    {}
};

class undo_redo_file
{
    std::fstream m_stream;
    const std::string m_fileActionName = "draw";
    const std::string m_fileCanvasName ="canvas";
    const std::string m_fileExtension = ".cache";
    std::string m_path;
    int m_IDpos = 0;
public:
    undo_redo_file();
    ~undo_redo_file();
    bool initialize(const QSize& imageSize);
private:
    bool createCacheFiles(const std::string& fileName, const int& count, const QSize& imageSize);
public:
    void set_appPath(const std::string &path);
    bool openAction();
    bool openAction(std::fstream& stream, const int & pos, bool rewrite = false);
    bool openCanvas(std::fstream& stream, const int & pos, bool rewrite = false);
    bool close();
    bool close(std::fstream& stream);

    bool set_to_cache(const UndoRedoFileArgs& args, cacheType ct = cacheType::action);
    bool get_from_cache(UndoRedoFileArgs& args, cacheType ct = cacheType::action);

};

#endif // UNDO_REDO_FILE_H
