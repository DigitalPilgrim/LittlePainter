#include "undo_redo_file.h"
#include <cstdio>
#include <QDebug>
#include <filesystem>

// ==============================================================================================================

undo_redo_file::undo_redo_file()
{

}

undo_redo_file::~undo_redo_file()
{
    if (!std::filesystem::is_empty(m_path))
    {
        for (auto& entry : std::filesystem::directory_iterator(m_path))
        {
            std::filesystem::remove_all(entry.path());
        }
    }
}

// ==============================================================================================================

bool undo_redo_file::initialize(const QSize& imageSize)
{
    qInfo () << "-- cache initialize() | BEGIN ---------------------";
    if (m_path.empty())
    {
        qInfo () << "-- cache initialize() | path empty!";
        qInfo () << "-- cache initialize() | END -----------------------";

        return false;
    }
    else
    {
        bool ok = false;
        if (!std::filesystem::exists(m_path))
        {
            qInfo () << "-- cache initialize() | directory not exists, create it.";
            std::filesystem::create_directory(m_path);
        }
        if (std::filesystem::exists(m_path))
        {
            qInfo () << "-- cache initialize() | path exists";
            if (!std::filesystem::is_empty(m_path))
            {
                qInfo () << "-- cache initialize() | path not empty, remove all";
                for (auto& entry : std::filesystem::directory_iterator(m_path))
                {
                    std::filesystem::remove_all(entry.path());
                }
                //std::filesystem::remove_all(m_path);
            }
            if (!std::filesystem::exists(m_path))
            {
                qInfo () << "-- cache initialize() | path was removed";
                return false;
            }

            ok = createCacheFiles(m_fileCanvasName, 10, imageSize);
            ok = createCacheFiles(m_fileActionName, 100, imageSize);
        }
        else
        {
            qInfo () << "-- cache initialize() | directory not exists, something happen.";
        }

        qInfo () << "-- cache initialize() | END -----------------------";
        return ok;
    }
}

// ==============================================================================================================

bool undo_redo_file::createCacheFiles(const std::string &fileName, const int& count, const QSize& imageSize)
{
    bool ok = false;
    std::fstream f;
    std::string path = m_path + fileName;
    std::string num;

    int32_t ID = -1;
    int32_t w = static_cast<int32_t>(imageSize.width());
    int32_t h = static_cast<int32_t>(imageSize.height());
    uint64_t b = 0;
    const int COUNT = count +1;
    for (int i = 1; i < COUNT; i++)
    {
        if (i < 1000)
        {
            num = "0";
            if (i < 100)
            {
                num += "0";
                if (i < 10)
                    num += "0";
            }
        }
        num += std::to_string(i);
        qInfo() << "-- cache file | " << (path + num + m_fileExtension).c_str();
        f.open(path + num + m_fileExtension, f.out | f.binary);
        num = "";
        if(f.is_open())
        {
            qInfo () << "-- cache initialize() | file opened";
            f.write((char *)&ID, sizeof(int32_t));
            f.write((char *)&w, sizeof(int32_t));
            f.write((char *)&h, sizeof(int32_t));
            f.write((char *)&b, sizeof(uint64_t));

            /*uint8_t R = 0, G = 0, B = 0, A = 255;

            for (int y = 0; y < imageSize.height(); y++)
            {
                for (int x = 0; x < imageSize.width(); x++)
                {
                    f.write((char *)&R, sizeof(uint8_t));
                    f.write((char *)&G, sizeof(uint8_t));
                    f.write((char *)&B, sizeof(uint8_t));
                    f.write((char *)&A, sizeof(uint8_t));
                }
            }*/

            ok = true;
            f.close();
        }
        else
        {
            qInfo () << "-- cache initialize() | file not opened!";
            ok = false;
            break;
        }
    }
    return ok;
}

// ==============================================================================================================

void undo_redo_file::set_appPath(const std::string &path)
{
    m_path = path; /* + "/" + m_fileName*/;
    qInfo() << "-- undo_redo_file path = " << m_path.c_str();

    //initialize();
}

// ==============================================================================================================

bool undo_redo_file::openAction()
{
    if (m_stream.is_open()) { return true; }
    else
    {
        m_stream.open(m_path.c_str(), m_stream.binary | m_stream.in | m_stream.out);
        if (m_stream.is_open()) return true;
        else return false;
    }
}

// ==============================================================================================================

bool undo_redo_file::openAction(std::fstream &stream, const int &pos, bool rewrite)
{
    std::string path = m_path + m_fileActionName;
    std::string num;
    if (pos < 1000) num = "0";
    if (pos < 100) num += "0";
    if (pos < 10) num += "0";
    num += std::to_string(pos);

    path = path + num + m_fileExtension;

    if (rewrite)
    {
        /*if (std::filesystem::exists(path))
        {
            std::filesystem::remove(path);
        }*/
        stream.open(path, stream.out | /*stream.in |*/ stream.binary | stream.trunc);
    }
    else
    {
        stream.open(path, /*stream.out |*/ stream.in | stream.binary );
    }

    qInfo() << "-- cache file OPEN | action | " << (m_fileActionName + num + m_fileExtension).c_str();

    //stream.open(path, stream.out | stream.in | stream.binary);
    if (stream.is_open()) {
        qInfo() << "-- cache file OPEN | file opened | " << (m_fileActionName + num + m_fileExtension).c_str();
        return true;
    }
    else {
        qInfo() << "-- cache file OPEN | file FAILED to open | " << (m_fileActionName + num + m_fileExtension).c_str();
        return false; }
}

// ==============================================================================================================

bool undo_redo_file::openCanvas(std::fstream &stream, const int &pos, bool rewrite)
{
    std::string path = m_path + m_fileCanvasName;
    std::string num;
    if (pos < 1000) num = "0";
    if (pos < 100) num += "0";
    if (pos < 10) num += "0";
    num += std::to_string(pos);

    path = path + num + m_fileExtension;
    if (rewrite)
    {
        /*if (std::filesystem::exists(path))
        {
            std::filesystem::remove(path);
        }*/
        stream.open(path, stream.out | /*stream.in |*/ stream.binary | stream.trunc);
    }
    else
    {
        stream.open(path, /*stream.out |*/ stream.in | stream.binary );
    }

    qInfo() << "-- cache file OPEN | canvas | " << (m_fileCanvasName + num + m_fileExtension).c_str();

    if (stream.is_open()) {
        qInfo() << "-- cache file OPEN | file opened | " << (m_fileCanvasName + num + m_fileExtension).c_str();
        return true;
    }
    else {
        qInfo() << "-- cache file OPEN | file FAILED to open | " << (m_fileCanvasName + num + m_fileExtension).c_str();
        return false; }
}

// ==============================================================================================================

bool undo_redo_file::close()
{
    m_stream.close();
    if (!m_stream.is_open()) return true;
    else return false;
}

// ==============================================================================================================

bool undo_redo_file::close(std::fstream &stream)
{
    stream.close();
    if (!stream.is_open()) return true;
    else return false;
}

// ==============================================================================================================

bool undo_redo_file::set_to_cache(const UndoRedoFileArgs &args, cacheType ct)
{
    std::fstream cache;
    bool opened = false;

    switch(ct)
    {
    case cacheType::action: opened = openAction(cache, args.Position, true); break;
    case cacheType::canvas: opened = openCanvas(cache, args.Position, true); break;
    }

    if (opened)
    {
        qInfo() << "-- cache file OPEN | set_to_cache | opened";
        if (args.Image != nullptr)
        {
            qInfo() << "-- cache file OPEN | if (args.Image != nullptr)"
            << " | size w(" << args.Image->size().width()
            << "), h(" << args.Image->size().height() << ")";

            cache.seekp(0);

            m_IDpos = args.Position;
            int32_t ID = args.Position;
            int32_t width = args.Image->size().width();
            int32_t height = args.Image->size().height();
            uint64_t bits = width * height * 4;
            cache.write((char *)&ID, sizeof(int32_t));
            cache.write((char*)&width, sizeof(int32_t));
            cache.write((char*)&height, sizeof(int32_t));
            cache.write((char*)&bits, sizeof(uint64_t));

            QColor c; uint8_t R = 0; uint8_t G = 0; uint8_t B = 0; uint8_t A = 0;


            for (int y = 0; y < args.Image->size().height(); y++)
            {
                for (int x = 0; x < args.Image->size().width(); x++)
                {
                    c = args.Image->pixelColor(x,y);
                    R = c.red();
                    G = c.green();
                    B = c.blue();
                    A = c.alpha();
                    cache.write((char *)&R, sizeof(uint8_t));
                    cache.write((char *)&G, sizeof(uint8_t));
                    cache.write((char *)&B, sizeof(uint8_t));
                    cache.write((char *)&A, sizeof(uint8_t));
                }
            }
            return close(cache);
        }
    }

    return false;
}

// ==============================================================================================================

bool undo_redo_file::get_from_cache(UndoRedoFileArgs &args, cacheType ct)
{
    bool ok = false;
    std::fstream cache;
    bool opened = false;

    switch(ct)
    {
    case cacheType::action: opened = openAction(cache, args.Position); break;
    case cacheType::canvas: opened = openCanvas(cache, args.Position); break;
    }

    if (opened)
    {
        qInfo() << "-- cache file OPEN | get_from_cache | opened";
        if (args.Image != nullptr)
        {
            m_stream.seekp(0);
            int32_t ID = 0;
            int32_t width = 0;
            int32_t height = 0;
            uint64_t bits = 0;

            QColor c; uint8_t R = 0; uint8_t G = 0; uint8_t B = 0; uint8_t A = 0;

            cache.read((char *)&ID, sizeof(int32_t));
            cache.read((char *)&width, sizeof(int32_t));
            cache.read((char *)&height, sizeof(int32_t));
            cache.read((char *)&bits, sizeof(uint64_t));
            if (static_cast<int>(ID) == args.Position)
            {
                m_IDpos = static_cast<int>(ID);
                int w = static_cast<int>(width);
                int h = static_cast<int>(height);
                *args.Image = QImage(QSize(w, h), QImage::Format_RGBA8888_Premultiplied);
                qInfo() << "-- get_from_cache() | ID = " << m_IDpos
                        << " | w " << w
                        << " | h " << h
                        << " | b " << static_cast<int>(bits);
                for (int y = 0; y < h; y++)
                {
                    for (int x = 0; x < w; x++)
                    {
                        cache.read((char *)&R, sizeof(uint8_t));
                        cache.read((char *)&G, sizeof(uint8_t));
                        cache.read((char *)&B, sizeof(uint8_t));
                        cache.read((char *)&A, sizeof(uint8_t));
                        c = QColor(static_cast<int>(R)
                                   , static_cast<int>(G)
                                   , static_cast<int>(B)
                                   , static_cast<int>(A));
                        args.Image->setPixelColor(x, y, c);
                    }
                }
                ok = true;
            }
        }
        close(cache);
    }
    return ok;
}

// ==============================================================================================================
