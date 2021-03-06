#include "undo_redo_file.h"
#include <cstdio>
#include <QDebug>
#include <filesystem>

#include <snappy-c.h>
//#include <snappy.h>

#include "timer_manager.h"

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
        bool ok = false;
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

            // COMPRESSION ---------------------------------------------------------
            if (m_compress) // ---------- use COMPRESSOR
            {
                //QColor c;

                unsigned long long compressTotalTime = 0;
                timer_manager::start();

                const size_t originalSIZE = (args.Image->size().width() * args.Image->size().height() * 4 );
                size_t compressSIZE = snappy_max_compressed_length(originalSIZE); //originalSIZE * 2; // * 2
                char * original = reinterpret_cast<char *>(args.Image->bits());//new char[originalSIZE];
                /*{
                    size_t p = 0;
                    for (int y = 0; y < args.Image->size().height(); y++)
                    {
                        for (int x = 0; x < args.Image->size().width(); x++)
                        {
                            c = args.Image->pixelColor(x,y);
                            original[p] = static_cast<char>(c.red());
                            original[p + 1] = static_cast<char>(c.green());
                            original[p + 2] = static_cast<char>(c.blue());
                            original[p + 3] = static_cast<char>(c.alpha());
                            p += 4;
                        }
                    }
                }*/
                char * compressed = new char[compressSIZE];

                timer_manager::stop();
                QString time;
                timer_manager::get_time(time);
                timer_manager::get_time(compressTotalTime);
                timer_manager::reset();

                qInfo() << "-- COMPRESSION | FILL BUFF | TIME = " << time;

                qInfo() << "-- COMPRESSION | BUFF SIZES BEFORE | compressSIZE = " << compressSIZE
                        << " | originalSIZE = " << originalSIZE;

                timer_manager::start();

                snappy_status st = snappy_compress(original, originalSIZE, compressed, &compressSIZE);


                timer_manager::stop();
                timer_manager::get_time(time);
                timer_manager::get_time(compressTotalTime);
                timer_manager::reset();

                qInfo() << "-- COMPRESSION | COMPRESSION TIME = " << time;

                if (st == snappy_status::SNAPPY_OK)
                {
                    qInfo() << "-- COMPRESSION | OK | compressSIZE = " << compressSIZE
                            << " | originalSIZE = " << originalSIZE;
                    timer_manager::start();
                    cache.write((char *)&compressSIZE, sizeof(size_t));
                    cache.write((char *)compressed, compressSIZE);
                    timer_manager::stop();
                    timer_manager::get_time(time);
                    timer_manager::get_time(compressTotalTime);
                    timer_manager::reset();

                    qInfo() << "-- COMPRESSION | WRITE TO FILE = " << time;

                    ok = true;
                }
                else if (st == snappy_status::SNAPPY_INVALID_INPUT)
                {
                    compressSIZE = 0;
                    cache.write((char *)&compressSIZE, sizeof(size_t));
                    qInfo() << "-- COMPRESSION | SNAPPY_INVALID_INPUT";
                }
                else if (st == snappy_status::SNAPPY_BUFFER_TOO_SMALL)
                {
                    compressSIZE = 0;
                    cache.write((char *)&compressSIZE, sizeof(size_t));
                    qInfo() << "-- COMPRESSION | SNAPPY_BUFFER_TOO_SMALL";
                }

                delete[] compressed;
                //delete[] original;

                timer_manager::get_time(time, compressTotalTime);
                qInfo() << "-- COMPRESSION | TOTAL TIME = " << time;
            }
            // -----------------------------------------------------------------------
            else            // ------ without COMPRESSOR
            {

                QColor c; uint8_t R = 0; uint8_t G = 0; uint8_t B = 0; uint8_t A = 0;

                timer_manager::start();

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

                timer_manager::stop();
                QString time;
                timer_manager::get_time(time);
                timer_manager::reset();
                qInfo() << "-- cache file | TIME WRITE TO FILE = " << time;

                ok = true;
            }
            // -----------------------------------------------------------------------
            close(cache);
            return ok;
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

            int w = static_cast<int>(width);
            int h = static_cast<int>(height);

            *args.Image = QImage(QSize(w, h), QImage::Format_RGBA8888_Premultiplied);

            // -----------------------------------------------------------------------
            if (m_compress) // ---------- use COMPRESSOR
            {
                size_t compressSIZE = 0;
                cache.read((char *)&compressSIZE, sizeof(size_t));

                if (compressSIZE > 0 && static_cast<int>(ID) == args.Position)
                {

                    qInfo() << "-- get_from_cache() | ID = " << m_IDpos
                            << " | w " << w
                            << " | h " << h
                            << " | b " << static_cast<int>(bits);

                    qInfo() << "-- DE COMPRESSION | compressSIZE = " << compressSIZE;

                    unsigned long long compressTotalTime = 0;
                    timer_manager::start();

                    size_t originalSIZE = bits;
                    unsigned char * uncompressed = args.Image->bits(); // new unsigned char[originalSIZE];
                    char * compressed = new char[compressSIZE];

                    cache.read((char *)compressed, compressSIZE);

                    timer_manager::stop();
                    QString time;
                    timer_manager::get_time(time);
                    timer_manager::get_time(compressTotalTime);
                    timer_manager::reset();

                    qInfo() << "-- DE COMPRESSION | READ FROM FILE = " << time;

                    timer_manager::start();

                    snappy_status st = snappy_uncompress(compressed, compressSIZE
                                                         , reinterpret_cast<char *>(uncompressed), &originalSIZE);

                    timer_manager::stop();
                    timer_manager::get_time(time);
                    timer_manager::get_time(compressTotalTime);
                    timer_manager::reset();

                    qInfo() << "-- DE COMPRESSION | DECOMPRESS TIME = " << time;

                    if (st == snappy_status::SNAPPY_OK)
                    {
                        qInfo() << "-- DE COMPRESSION | OK | compressSIZE = " << compressSIZE
                                << " | originalSIZE = " << originalSIZE;

                        timer_manager::start();

                        /*size_t p = 0;
                        //*args.Image = QImage(QSize(w, h), QImage::Format_RGBA8888_Premultiplied);
                        for (int y = 0; y < h; y++)
                        {
                            for (int x = 0; x < w; x++)
                            {
                                // ----------------------------------
                                // neviem ci je toto v poriadku, ale funguje to
                                // uint8_t = char;
                                R = uncompressed[p];
                                G = uncompressed[p + 1];
                                B = uncompressed[p + 2];
                                A = uncompressed[p + 3];
                                // ----------------------------------
                                // ak je QColor(static_cast<int>(uncompressed[p])
                                //      int z static_cast<int>(char) nefunguje, pise invalid color.
                                c = QColor(static_cast<int>(R)
                                           , static_cast<int>(G)
                                           , static_cast<int>(B)
                                           , static_cast<int>(A));
                                args.Image->setPixelColor(x, y, c);
                                p += 4;
                            }
                        }*/

                        timer_manager::stop();
                        timer_manager::get_time(time);
                        timer_manager::get_time(compressTotalTime);
                        timer_manager::reset();

                        qInfo() << "-- DE COMPRESSION | FILL QImage TIME = " << time;

                        ok = true;
                    }
                    else if (st == snappy_status::SNAPPY_INVALID_INPUT)
                    {
                        qInfo() << "-- DE COMPRESSION | SNAPPY_INVALID_INPUT";
                    }
                    else if (st == snappy_status::SNAPPY_BUFFER_TOO_SMALL)
                    {
                        qInfo() << "-- DE COMPRESSION | SNAPPY_BUFFER_TOO_SMALL";
                    }

                    //delete[] uncompressed;
                    delete[] compressed;

                    timer_manager::get_time(time, compressTotalTime);
                    qInfo() << "-- DE COMPRESSION | TOTAL TIME = " << time;

                }
                else
                {
                    qInfo() << "-- DE COMPRESSION | FAIL | compressSIZE = " << compressSIZE;
                }
            }
            // -----------------------------------------------------------------------
            else            // ------ without COMPRESSOR
            {
                if (static_cast<int>(ID) == args.Position)
                {

                    timer_manager::start();

                    m_IDpos = static_cast<int>(ID);
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

                    timer_manager::stop();
                    QString time;
                    timer_manager::get_time(time);
                    timer_manager::reset();

                    qInfo() << "-- cache file | READ FROM FILE = " << time;

                    ok = true;
                }
            }
            // -----------------------------------------------------------------------
        }
        close(cache);
    }
    return ok;
}

// ==============================================================================================================
