#include "undo_redo_special.h"
#include <QDebug>
#include "painter_brush_debug.h"
#include "graphic_helper_functions.h"

// ----------------------------------------------------------------------------------------------------------------

undo_redo_special::undo_redo_special()
{

}

// ----------------------------------------------------------------------------------------------------------------

void undo_redo_special::setFileCachePath(std::string path)
{
    m_fileCache.set_appPath(path);
    qInfo () << "-- cache initialize() | setFileCachePath()";
    //m_fileCache.initialize();
}

// ================================================================================================================
// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ================================================================================================================

// OLD UNDO (NEPOUZIVAT sluzi na uchovanie starsej verzie undo)

// ----------------------------------------------------------------------------------------------------------------

bool undo_redo_special::oldUndo(UndoRedoSpecialArgs &args)
{
    if (availableUndo())
    {
        //args.Image->fill(QColor(0, 0, 0, 0));
        QPainter painter(args.Image);
        painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

        std::list<DrawHistory>::iterator it = --m_history.end();
        size_t size = m_history.size();

        for (size_t i = 0; i < size; i++)
        {
            if (it->ID == m_hPos)
            {
                --m_hPos;
                args.Area = it->Area;
                break;

            }
            else --it;
        }

        for (DrawHistory & dh : m_history)
        {
            if (dh.ID <= m_hPos)
            {
                if (dh.Action == historyActions::draw)
                {
                    //debug_paint_brush(dh.Draw.Brush());
                    QPoint lastPoint = dh.Moves.front();
                    QPoint measurePoint = dh.Moves.front();
                    if (dh.DrawCached)
                    {
                        painter.drawImage(QPoint(dh.Area.Left, dh.Area.Top)
                                          , dh.StoredImage);
                    }
                    else
                    {
                        for ( QPoint & point : dh.Moves)
                        {
                            QRect area;
                            DrawManagerArgs largs(&painter, &point, &area, &lastPoint, &measurePoint);
                            dh.Draw.draw(largs);
                            lastPoint = point;
                        }
                    }
                }
                else if (dh.Action == historyActions::fill)
                {
                    painter.fillRect(QRect(args.Area.Left, args.Area.Top, args.Area.Right, args.Area.Bottom), dh.Fill);
                }
                else if (dh.Action == historyActions::store_image)
                {
                    painter.drawImage(QRect(args.Area.Left, args.Area.Top, args.Area.Right, args.Area.Bottom)
                                      , dh.StoredImage
                                      , QRect(0, 0, dh.StoredImage.size().width(), dh.StoredImage.size().height()));
                }

                qInfo() << "-- undo(args) | catch | id = " << dh.ID;
            }
            else break;
        }

        painter.end();
        return true;
    }
    qInfo() << "-- undo(args) | failed | pos = " << m_hPos;
    return false;
}

// ================================================================================================================
// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ================================================================================================================

// UNDO

// ----------------------------------------------------------------------------------------------------------------

bool undo_redo_special::undo(UndoRedoSpecialArgs &args)
{
    if (availableUndo())
    {

        // -------------------------------------------------------------------------------------------------
        // nastav cache dozadu
        // false = backward
        undoRedoCache(args, false);
        // -------------------------------------------------------------------------------------------------
        QPainter painter(args.Image);
        painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

        // -------------------------------------------------------------------------------------------------
        // Zober oblast tahu stetca ktory bol prave nakresleny (Area)
        std::list<DrawHistory>::iterator it = --m_history.end();
        size_t size = m_history.size();

        for (size_t i = 0; i < size; i++)
        {
            if (it->ID == m_hPos)
            {
                --m_hPos;
                args.Area = it->Area;
                break;

            }
            else --it;
        }

        // -------------------------------------------------------------------------------------------------
        // vycisti cast platna (z Area)
        it = m_history.begin();
        if (it->Action == historyActions::fill)
        {
            painter.fillRect(QRect(args.Area.Left, args.Area.Top, args.Area.Right, args.Area.Bottom), it->Fill);
        }
        else if (it->Action == historyActions::store_image)
        {
            painter.drawImage(QRect(args.Area.Left, args.Area.Top, args.Area.Right, args.Area.Bottom)
                              , it->StoredImage
                              , QRect(0, 0, it->StoredImage.size().width(), it->StoredImage.size().height()));
        }

        // -------------------------------------------------------------------------------------------------
        // zober oblast z cache platna (Cache Canvas) ak je to mozne
        int fromPos = 0;
        std::string debug_useCanvasCache = "false";
        if (m_useCanvasCache) // m_hPos <= (m_canvas_cache_MAX - MAX_CANVAS_CACHE)
        {
            debug_useCanvasCache = "true";
            fromPos = (m_hPos - MAX_CANVAS_CACHE) - 1;
            painter.drawImage(QRect(args.Area.Left, args.Area.Top, args.Area.Right, args.Area.Bottom)
                              , m_activeCanvasCache->Canvas
                              , QRect(args.Area.Left, args.Area.Top, args.Area.Right, args.Area.Bottom));
        }
        qInfo() << "-- UNDO | fromPos = " << fromPos
                << " | m_useCanvasCache = " << debug_useCanvasCache.c_str()
                << " | Canvas ID = " << m_activeCanvasCache->ID
                << " | m_hPos = " << m_hPos
                   ;
        // -------------------------------------------------------------------------------------------------
        // vykresli ostatne tahy z historie
        if (fromPos > 0) std::advance(it, fromPos);
        while (it != m_history.end()) // DrawHistory & dh : m_history
        {
            if (it->ID <= m_hPos)
            {
                if (it->Action == historyActions::draw)
                {
                    //debug_paint_brush(it->Draw.Brush());
                    QPoint lastPoint = it->Moves.front();
                    QPoint measurePoint = it->Moves.front();
                    if (it->DrawCached)
                    {
                        qInfo() << "-- undo(args) | catch | id = " << it->ID
                                << " | draw from cache";
                        if (it->StoredImage.isNull())
                        {
                            qInfo() << "-- undo(args) | catch | id = " << it->ID
                                    << " | StoredImage IS NULL !";
                        }
                        painter.drawImage(QPoint(it->Area.Left, it->Area.Top)
                                          , it->StoredImage);
                    }
                    else
                    {
                        if (it->Moves.size() > 0 )
                        {
                            qInfo() << "-- undo(args) | catch | id = " << it->ID
                                    << " | draw from action";
                        }
                        if (it->Cached)
                        {
                            qInfo() << "-- undo(args) | catch | id = " << it->ID
                                    << " | draw is cached";
                        }
                        for ( QPoint & point : it->Moves)
                        {
                            QRect area;
                            DrawManagerArgs largs(&painter, &point, &area, &lastPoint, &measurePoint);
                            it->Draw.draw(largs);
                            lastPoint = point;
                        }
                    }
                }

                qInfo() << "-- undo(args) | catch | id = " << it->ID;
            }
            else break;
            ++it;
        }

        // -------------------------------------------------------------------------------------------------

        painter.end();

        // -------------------------------------------------------------------------------------------------

        return true;
    }
    qInfo() << "-- undo(args) | UNDO NOT AVAILABLE | pos = " << m_hPos;
    return false;
}

// ================================================================================================================
// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ================================================================================================================

// REDO

// ----------------------------------------------------------------------------------------------------------------

bool undo_redo_special::redo(UndoRedoSpecialArgs &args)
{
    /*
     * INFO:
        Pri redo nie je umplementovane historyActions::fill ani historyActions::store_image
        pretoze, aj tak pri nacitani obrazka alebo vycistenia platna, sa resetne undo / redo
    */

    if (availableRedo())
    {

        // -------------------------------------------------------------------------------------------------
        // nastav cache dopredu
        // true = forward
        undoRedoCache(args, true);
        // -------------------------------------------------------------------------------------------------
        ++m_hPos;
        QPainter painter(args.Image);
        painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
        for (DrawHistory & dh : m_history)
        {
            if (dh.ID == m_hPos)
            {
                if (dh.Action == historyActions::draw)
                {
                    args.Area = dh.Area;
                    QPoint lastPoint = dh.Moves.front();
                    QPoint measurePoint = dh.Moves.front();
                    if (dh.DrawCached)
                    {
                        QRect r = QRect(args.Area.Left, args.Area.Top, args.Area.Right, args.Area.Bottom);
                        QRect s = QRect(dh.Area.Left, dh.Area.Top, dh.Area.Right, dh.Area.Bottom);
                        painter.drawImage(r, dh.StoredImage
                                          //, s
                                          );
                    }
                    else
                    {
                        for ( QPoint & point : dh.Moves)
                        {
                            QRect area;
                            DrawManagerArgs args(&painter, &point, &area, &lastPoint, &measurePoint);
                            dh.Draw.draw(args);
                            lastPoint = point;
                        }
                    }
                }

                qInfo() << "-- redo(args) | catch | pos = " << m_hPos;
            }
            else if (dh.ID > m_hPos) break;
        }

        painter.end();
        return true;
    }


    qInfo() << "-- redo(args) | REDO NOT AVAILABLE | pos = " << m_hPos;
    return false;
}

// ================================================================================================================
// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ================================================================================================================

// UNDO CACHE

// ----------------------------------------------------------------------------------------------------------------

bool undo_redo_special::undoRedoCache(UndoRedoSpecialArgs &args, bool Forward)
{
    // CACHE           V
    // files< 0, 1, 2, 3, 4, 5, 6, 7 >
    // posBack   <-----------> posForward
    //                 |
    //                 |
    //                 V
    //             m_cachePos

    // ---------------------------------------------------------------------------------------
    //bool setCache = false;
   // bool setCanvasCache = false;
    m_useCanvasCache = false;
    if (Forward)    // >>>
    {
        ++m_cachePos;   // ACTIONS & DRAW FILE CACHE
        ++m_canvas_cache; // CANVAS FILE CACHE
        std::string debug_useCanvasCache = "false";
        // ----------------------------------------------------------------------------------
        // Tahy stetcom
        if (m_cachePos > m_max_cache)
        {
            m_max_cache = m_max_cache + MAX_CACHE;
        }
        // ----------------------------------------------------------------------------------
        // Canvas
        if (m_canvas_cache > m_canvas_cache_MAX)
        {
            ++m_canvas_cache_pos;
            m_canvas_cache_MAX = m_canvas_cache_MAX + MAX_CANVAS_CACHE;
            m_useCanvasCache = true;
        }
        // ----------------------------------------------------------------------------------
        if (m_useCanvasCache) debug_useCanvasCache = "true";
        qInfo() << "-- undoRedoCache() | FORWARD | --------------------";
        qInfo() << " | m_cachePos = " << m_cachePos
                << " | m_max_cache = " << m_max_cache
                << " | m_canvas_cache = " << m_canvas_cache
                << " | m_canvas_cache_pos = " << m_canvas_cache_pos
                << " | m_useCanvasCache = " << debug_useCanvasCache.c_str();
        qInfo() << "-- ";
    }
    else            // <<< Backward
    {
        --m_cachePos;   // ACTIONS & DRAW FILE CACHE
        --m_canvas_cache; // CANVAS FILE CACHE
        std::string debug_useCanvasCache = "false";
        // ----------------------------------------------------------------------------------
        // Tahy stetcom
        if (m_cachePos <= (m_max_cache - MAX_CACHE))
        {
            if ((m_max_cache - MAX_CACHE) > 0) m_max_cache -= MAX_CACHE;
        }
        // ----------------------------------------------------------------------------------
        // Canvas

        if (m_canvas_cache >= (m_canvas_cache_MAX - MAX_CANVAS_CACHE) && (m_canvas_cache_MAX - MAX_CANVAS_CACHE) > 0)
        {
            m_useCanvasCache = true;
        }
        if (m_canvas_cache <= (m_canvas_cache_MAX - MAX_CANVAS_CACHE)) // m_canvas_cache <= (m_canvas_cache_MAX - MAX_CANVAS_CACHE)
        {
            if ((m_canvas_cache_MAX - MAX_CANVAS_CACHE) > 0)
            {
                --m_canvas_cache_pos;
                m_canvas_cache_MAX = m_canvas_cache_MAX - MAX_CANVAS_CACHE;

            }
            else m_useCanvasCache = false;
        }
        // ----------------------------------------------------------------------------------
        if (m_useCanvasCache) debug_useCanvasCache = "true";
        qInfo() << "-- undoRedoCache() | BACKWARD | --------------------";
        qInfo() << " | m_cachePos = " << m_cachePos
                << " | m_max_cache = " << m_max_cache
                << " | m_canvas_cache = " << m_canvas_cache
                << " | m_canvas_cache_pos = " << m_canvas_cache_pos
                << " | m_canvas_cache_MAX = " << m_canvas_cache_MAX
                << " | m_useCanvasCache = " << debug_useCanvasCache.c_str();
        qInfo() << "-- ";
    }
    // ---------------------------------------------------------------------------------------
    // DRAW CACHE
    //if (setCache)
    {
        // Vzdy sa nastavuje aj forward aj backward sucasne
        // ------------------------------------------------------------------------------------
        // FORWARD
        int posForward = (m_cachePos + MAX_CACHE); // - 1 ID zacina od 1 ale std::advance ide od 0
        if (Forward) posForward -= 1; // potrebne, inak bude nacitavat aukladat nespravne tahy
        if (posForward < m_history.size())
        {
            std::list<DrawHistory>::iterator it = m_history.begin();
            std::advance(it, posForward);
            //posForward += 2;  // spat na ID hodnoty
            ++posForward;  // spat na ID hodnoty
            qInfo() << "-- posForward = " << posForward;
            if (it->ID == posForward)
            {
                if (Forward)
                {
                    if (it->Cached && it->DrawCached)
                    {
                        UndoRedoFileArgs args = UndoRedoFileArgs(&it->StoredImage, posForward);
                        if (m_fileCache.get_from_cache(args))
                        {
                            it->Cached = false;
                            qInfo() << "-- CACHE >>> FORWARD | SUCCESS";
                        }
                        else
                        {
                            qInfo() << "-- CACHE >>> FORWARD | FAILED";
                        }
                    }
                }
                else
                {
                    if (it->DrawCached && it->Cached == false)
                    {
                        if (m_fileCache.set_to_cache(UndoRedoFileArgs(&it->StoredImage, posForward)))
                        {
                            it->Cached = true;
                            it->StoredImage = QImage();
                            qInfo() << "-- CACHE <<< FORWARD | SUCCESS";
                        }
                        else
                        {
                            qInfo() << "-- CACHE <<< FORWARD | FAILED";
                        }
                    }
                }

            }
        }
        // ------------------------------------------------------------------------------------
        // BACKWARD
        int posBackward = (m_cachePos - MAX_CACHE); // ID zacina od 1 ale std::advance ide od 0
        if (Forward) posBackward -= 1; // potrebne, inak bude nacitavat aukladat nespravne tahy
        if (posBackward > 0)
        {
            std::list<DrawHistory>::iterator it = m_history.begin();
            std::advance(it, posBackward);
            ++posBackward;  // spat na ID hodnoty
            qInfo() << "-- posBackward = " << posBackward;
            if (it->ID == posBackward)
            {
                if (Forward)
                {
                    if (it->Cached == false && it->DrawCached)
                    {
                        if (m_fileCache.set_to_cache(UndoRedoFileArgs(&it->StoredImage, posBackward)))
                        {
                            it->Cached = true;
                            it->StoredImage = QImage();
                            qInfo() << "-- CACHE >>> SET BACKWARD | SUCCESS";
                        }
                        else
                        {
                            qInfo() << "-- CACHE >>> SET BACKWARD | FAILED";
                        }
                    }
                }
                else
                {
                    if (it->Cached && it->DrawCached)
                    {
                        UndoRedoFileArgs args = UndoRedoFileArgs(&it->StoredImage, posBackward);
                        if (m_fileCache.get_from_cache(args))
                        {
                            it->Cached = false;
                            qInfo() << "-- CACHE <<< GET BACKWARD | SUCCESS";
                        }
                        else
                        {
                            qInfo() << "-- CACHE <<< GET BACKWARD | FAILED";
                        }
                    }
                }

            }
        } // end if (posBackward > 0)

    } // end if (m_cachePos < (m_max_cache - MAX_CACHE))
    // ------------------------------------------------------------------------------------
    // CANVAS CACHE
    // ------------------------------------------------------------------------------------
    //
    if (m_canvas_cache_pos != m_activeCanvasCache->ID)
    {
        std::list<CanvasHistory>::iterator it = m_historyCanvasCache.begin();
        int pos = m_canvas_cache_pos - 1;

        qInfo() << "-- if (m_canvas_cache_pos != m_activeCanvasCache->ID) | "
                << "m_canvas_cache_pos = " << m_canvas_cache_pos
                << " | pos = " << pos;

        if (pos < m_historyCanvasCache.size()) // pos > 0 &&
        {
            if (pos > 0) std::advance(it, pos);
            qInfo() << "-- Canvas | " << "ID = " << it->ID
                    << " | Current Canvas ID = " << m_activeCanvasCache->ID;

            // nove platno zalohuj na disk
            if (m_activeCanvasCache->Cached == false)
            {
                qInfo() << "-- Canvas | NOT CACHED";
                m_fileCache.set_to_cache(UndoRedoFileArgs(&m_activeCanvasCache->Canvas, m_activeCanvasCache->ID)
                                         , cacheType::canvas);
                m_activeCanvasCache->Cached = true;

            }
            // vymaz image z pamate
            m_activeCanvasCache->Canvas = QImage();
            // prehodit na nove / stare platno
            m_activeCanvasCache = &(*it);
            UndoRedoFileArgs args = UndoRedoFileArgs(&m_activeCanvasCache->Canvas, m_activeCanvasCache->ID);
            m_fileCache.get_from_cache(args, cacheType::canvas);

            qInfo() << "-- Canvas NEW | " << "ID = " << it->ID;
        }

    }
    // ------------------------------------------------------------------------------------


    return true;
}

// ================================================================================================================
// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ================================================================================================================

// SET CACHE

// ----------------------------------------------------------------------------------------------------------------

void undo_redo_special::setCache()
{
    // ---------------------------------------------------------------------------
    ++m_cachePos;
    ++m_canvas_cache;
    bool resetCache = false;
    if (m_cachePos > m_max_cache)
    {
        int debug_max_cache = m_max_cache;
        m_max_cache = m_max_cache + MAX_CACHE;

        qInfo() << "-- CACHE | Set | cache pos = " << m_cachePos
                << " | m_max_cache before = " << debug_max_cache
                << " | m_max_cache after = " << m_max_cache;

        // --------------------------------------------------------------------------
        // Vlazit platno(Canvas) do suboru
        {
            if (m_canvas_cache > m_canvas_cache_MAX)
            {
                int debug_canvas_cache_MAX = m_canvas_cache_MAX;
                m_canvas_cache_MAX = m_canvas_cache_MAX + MAX_CANVAS_CACHE;


                m_useCanvasCache = true;
                ++m_canvas_cache_pos;

                qInfo() << "-- CANVAS CACHE | Set | m_canvas_cache = " << m_canvas_cache
                        << " | m_canvas_cache_MAX before = " << debug_canvas_cache_MAX
                        << " | m_canvas_cache_MAX  after= " << m_canvas_cache_MAX
                        << " | m_canvas_cache_pos = " << m_canvas_cache_pos
                        << " | Canvas ID = " << m_activeCanvasCache->ID;

                if (m_canvas_cache_pos > m_historyCanvasCache.size())
                {
                    m_fileCache.set_to_cache(UndoRedoFileArgs(&m_activeCanvasCache->Canvas, m_activeCanvasCache->ID)
                                          , cacheType::canvas);
                    // -------------------------------------------------------------------
                    // tento postup musi byt tak ako je inak to nebude fungovat
                    // -------------------------------------------------------------------
                    // 1. najprv vlozit do m_historyCanvasCache aby sa zachoval obrazok
                    m_historyCanvasCache.push_back(CanvasHistory(m_canvas_cache_pos, m_activeCanvasCache->Canvas));
                    // 2. vynulovat canvas z predosleho CanvasHistory
                    m_activeCanvasCache->Canvas = QImage();
                    m_activeCanvasCache->Cached = true;
                    // 3. zobrat posledny (vlozeny) CanvasHistory ktory sa bude pouzivat
                    m_activeCanvasCache = &m_historyCanvasCache.back();
                    // -------------------------------------------------------------------
                    qInfo() << "-- CANVAS CACHE | pushed to back |"
                            << "Canvas ID = " << m_activeCanvasCache->ID
                            << " | m_canvas_cache_pos = " << m_canvas_cache_pos
                            << " | m_historyCanvasCache.size() = " << m_historyCanvasCache.size()
                            << " | ----------------";
                }
                else
                {
                    resetCache = true;
                }

            }
        }
        // --------------------------------------------------------------------------
    }
    // ---------------------------------------------------------------------------
    // ak historia nie je na konci a spravi sa tah, resetne sa cache tak
    // ze od noveho tahu sa nastavi to spravne platno (aby potom zobrazovalo spravne tahy)
    if (resetCache || (m_hPos < m_hPosMax) )
    {
        m_hPosMax = m_hPos;
        qInfo() << "-- CANVAS CACHE | RESET ---------";

        // -----------------------------------------------------------------------
        // zober poziciu na ktorej sa nachadza platno
        // napr. ak je pozicia m_canvas_cache na 15, platno musi byt na pozicii 2 (pre std::list<> je to 1)
        // cize 1 = std::floor(15 / 10)
        int canvasPos = std::floor(static_cast<double>(m_canvas_cache) / static_cast<double>(MAX_CANVAS_CACHE));

        //pos = m_canvas_cache_pos - 1; // std::advance zacina od 0
        std::list<CanvasHistory>::iterator it = m_historyCanvasCache.begin();
        if (canvasPos > 0) std::advance(it, canvasPos);
        if (m_activeCanvasCache->ID != it->ID)
        {
            m_activeCanvasCache->Canvas = QImage();
            m_activeCanvasCache = &(*it);
            UndoRedoFileArgs args = UndoRedoFileArgs(&m_activeCanvasCache->Canvas, m_activeCanvasCache->ID);
            m_fileCache.get_from_cache(args, cacheType::canvas); //
            m_activeCanvasCache->Cached = true;
        }
        else //if (m_activeCanvasCache->ID == it->ID)
        {
            // ak m_activeCanvasCache sa nachadza na pozicii v ktorej treba spravit zmeny
            // napr. undo / redo poziciia bola na 19 a teraz je na 12 a spravi sa novy tah
            // m_activeCanvasCache je na pozicii 2 (ci uz na 19tom alebo 12tom tahu).
        }

        m_canvas_cache_pos = m_activeCanvasCache->ID;

        std::string canvasCahced = "false";
        if (m_activeCanvasCache->Cached) canvasCahced = "true";
        qInfo() << "-- CANVAS CACHE | NASTAVENE NA ID = " << m_activeCanvasCache->ID << " | canvasPos = " << canvasPos
                << " | canvasCahced = " << canvasCahced.c_str();
        // -----------------------------------------------------------------------
        // vyresetovat cache ak je potrebne
        if ((m_activeCanvasCache->ID -1) < m_historyCanvasCache.size())
        {
            std::list<CanvasHistory>::iterator cit = m_historyCanvasCache.begin();
            int citPos = m_activeCanvasCache->ID -1;
            if (citPos > 0) std::advance(cit, citPos);
            while (cit != m_historyCanvasCache.end())
            {
                if (cit->ID <= m_activeCanvasCache->ID)
                {
                    ++cit;
                }
                else
                {
                     qInfo() << "-- CANVAS CACHE | ID = " << cit->ID << " will be deleted";
                    cit = m_historyCanvasCache.erase(cit);
                }
            }
            /*for (CanvasHistory & ch : m_historyCanvasCache)
            {
                if (ch.ID > m_activeCanvasCache->ID)
                {
                    ch.Cached = false;
                    qInfo() << "-- CANVAS CACHE | ID = " << ch.ID << " cache reset";
                }
            }*/
        }

        // -----------------------------------------------------------------------
        // vytvorit aby dokreslilo spatne 10 tahov dozadu na platno
        // !!! ZBYTOCNE !!!
        // ale pre istotu to zatial necham tu
        //if (m_canvas_cache <= MAX_CANVAS_CACHE)
        // mozno nepotrebne
        /*int drawPos = (m_canvas_cache - MAX_CANVAS_CACHE);
        if (drawPos > 0)
        {

            QPainter p(&m_activeCanvasCache->Canvas);
            std::list<DrawHistory>::iterator drawIt = m_history.begin();
            // canvasPos sa rata v 10tkach lebo MAX_CANVAS_CACHE je 10
            // cize ak undo / redo pozicia bola na hodnote 23, od noveho tahu sa musi od 10 po 13
            int minDrawPos = canvasPos * MAX_CANVAS_CACHE;
            if (minDrawPos < 0) minDrawPos = 0;
            if (minDrawPos > 0) std::advance(drawIt, minDrawPos);
            qInfo() << "-- CANVAS CACHE | drawPos = " << drawPos << " | minDrawPos = " << minDrawPos;
            for (int i = minDrawPos; i < drawPos; i++)
            {
                if (drawIt->Action == historyActions::draw)
                {
                    bool backToCache = false;
                    if (!drawIt->Cached)
                    {
                        UndoRedoFileArgs args = UndoRedoFileArgs(&drawIt->StoredImage, drawIt->ID);
                        m_fileCache.get_from_cache(args);
                        backToCache = true;
                    }

                    if (drawIt->DrawCached)
                    {
                        p.drawImage(QRect(
                                    drawIt->Area.Left
                                    , drawIt->Area.Top
                                    , drawIt->Area.Right
                                    , drawIt->Area.Bottom)
                                    , drawIt->StoredImage);
                    }
                    else
                    {
                        QPoint lastPoint = drawIt->Moves.front();
                        QPoint measurePoint = drawIt->Moves.front();
                        for ( QPoint & point : drawIt->Moves)
                        {
                            QRect area;
                            DrawManagerArgs args(&p, &point, &area, &lastPoint, &measurePoint);
                            drawIt->Draw.draw(args);
                            lastPoint = point;
                        }
                        drawIt->Cached = true;
                    }

                    if (backToCache)
                    {
                        m_fileCache.set_to_cache(UndoRedoFileArgs(&drawIt->StoredImage, drawIt->ID));
                        drawIt->StoredImage = QImage();
                    }

                    qInfo() << "-- CACHE | drawI->ID " << drawIt->ID << " draw into canvas";
                }

                ++drawIt;
            }

            p.end();
        }
        else // m_historyCanvasCache ma poziciu 1 a ziadne tahy este nie su vlozene
        {
           // m_activeCanvasCache->Canvas.fill(QColor(255, 255, 255, 255));
        }*/
        // -----------------------------------------------------------------------


    }
    // --------------------------------------------------------------------------
    // Namalovat 10ty (MAX_CANVAS_CACHE) tah dozadu na Canvas
    // a 10ty tah ulozit do suboru (file cache)
    int pos = (m_hPos - MAX_CANVAS_CACHE) - 1; //  - 1
    if (pos > 0)
    {
        qInfo() << "-- DRAW TO CACHE CANVAS AND STORE | POS = " << pos;
        QPainter p(&m_activeCanvasCache->Canvas);
        //p.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
        for (DrawHistory & dh : m_history)
        {
            if (dh.ID == pos)
            {
                if (dh.Action == historyActions::draw)
                {
                    if (dh.DrawCached)
                    {
                        // vykresluje sa len v pripade, ze platno je "nove", teda este nie je ulozene na disku
                        // to pre pripad, ze sa islo niekolko krokov spat, avsak uz malovanie nie je potrebne
                        // do platna
                        if (m_activeCanvasCache->Cached == false)
                        {
                            qInfo() << "-- DRAW TO CACHE FROM IMAGE | TRUE ID = " << dh.ID;
                            p.drawImage(QRect(dh.Area.Left, dh.Area.Top, dh.Area.Right, dh.Area.Bottom), dh.StoredImage);
                        }
                        m_fileCache.set_to_cache(UndoRedoFileArgs(&dh.StoredImage, dh.ID));
                        dh.StoredImage = QImage();
                        dh.Cached = true;
                    }
                    else
                    {
                        // vykresluje sa len v pripade, ze platno je "nove", teda este nie je ulozene na disku
                        // ...
                        if (m_activeCanvasCache->Cached == false)
                        {
                            qInfo() << "-- DRAW TO CACHE FROM ACTION | TRUE ID = " << dh.ID;
                            QPoint lastPoint = dh.Moves.front();
                            QPoint measurePoint = dh.Moves.front();
                            for ( QPoint & point : dh.Moves)
                            {
                                QRect area;
                                DrawManagerArgs args(&p, &point, &area, &lastPoint, &measurePoint);
                                dh.Draw.draw(args);
                                lastPoint = point;
                            }
                        }
                        dh.Cached = true;
                    }
                }
                else if (dh.Action == historyActions::fill)
                {
                    if (m_activeCanvasCache->Cached == false)
                        p.fillRect(QRect(dh.Area.Left, dh.Area.Top, dh.Area.Right, dh.Area.Bottom), dh.Fill);
                }
                else if (dh.Action == historyActions::store_image)
                {
                    if (m_activeCanvasCache->Cached == false)
                        p.drawImage(QPoint(dh.Area.Left, dh.Area.Top), dh.StoredImage);
                }
                break;
            }
        }

        p.end();
    }
    // --------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    //qInfo() << "-- CANVAS CACHE | loaded from m_historyCanvasCache | pos = " << pos ;
    // ---------------------------------------------------------------------------
    qInfo() << "setCache() | m_hPos = " << m_hPos
            << " | m_cachePos = " << m_cachePos
            << " | m_canvas_cache_pos = " << m_canvas_cache_pos
               ;
}

// ================================================================================================================
// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ================================================================================================================

// setINI

// ----------------------------------------------------------------------------------------------------------------

bool undo_redo_special::setINI(const UndoRedoSpecialSetIniArgs &args)
{
    m_imageSize = args.ImageSize;

    m_history.clear();
    m_historyCanvasCache.clear();

    m_historyActive = nullptr;
    m_activeCanvasCache = nullptr;

    m_hPos = 0;
    m_hPosMax = 0;
    m_canvas_cache = 0;
    m_canvas_cache_MAX = MAX_CANVAS_CACHE;
    m_canvas_cache_pos = 1;
    m_max_cache = MAX_CACHE;
    m_cachePos = 0;

    m_fileCache.initialize(args.ImageSize);

    if (args.Image != nullptr)
    {
        m_historyCanvasCache.push_back(CanvasHistory(m_canvas_cache_pos, *args.Image));
        m_activeCanvasCache = &m_historyCanvasCache.front();
    }
    else
    {
        m_historyCanvasCache.push_back(CanvasHistory(m_canvas_cache_pos, QImage(args.ImageSize, QImage::Format_RGBA8888_Premultiplied)));
        m_activeCanvasCache = &m_historyCanvasCache.front();
    }

    return true;
}

// ================================================================================================================
// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ================================================================================================================

// setBegin

// ----------------------------------------------------------------------------------------------------------------

bool undo_redo_special::setBegin(const UndoRedoSpecialBeginArgs &args)
{
    qInfo() << "--------------------------------------------------------------";
    qInfo() << "-- setBegin()";
    if (m_historyActive == nullptr)
    {
        m_draw_moves = 0;
        m_drawIntoCache = false;
        ++m_hPos;
        // ---------------------------------------------------------------------------
        // DRAW HISTORY
        if (m_hPos > m_hPosMax)
        {
            if (args.Action == historyActions::draw)
            {
                //debug_paint_brush(args.Brush);
                m_history.push_back(DrawHistory(m_hPos, PainterBrushHolder(args.Brush)));
                m_historyActive = &m_history.back();
            }
            else if (args.Action == historyActions::fill)
            {
                m_history.push_back(DrawHistory(m_hPos, *args.Fill));
            }
            else if (args.Action == historyActions::store_image)
            {
                m_history.push_back(DrawHistory(m_hPos, *args.StoreImage));
            }
            m_hPosMax = m_hPos;
        }
        else if (m_hPos <= m_hPosMax)
        {
            qInfo() << "-- change a existing Brush in history | m_hPos = " << m_hPos;

            for (DrawHistory & dh : m_history)
            {
                if (dh.ID == m_hPos)
                {
                    dh.Moves.clear();
                    dh.StoredImage = QImage();
                    dh.DrawCached = false;
                    dh.Area = { 0, 0, 0, 0 };
                    if (args.Action == historyActions::draw)
                    {
                        //debug_paint_brush(args.Brush);
                        dh.Draw.set(args.Brush);
                        dh.Action = historyActions::draw;
                        m_historyActive = &dh;

                        qInfo() << "-- Brush cahnged | ID = " << dh.ID;
                    }
                    else if (args.Action == historyActions::fill)
                    {
                        dh.Draw.release();
                        dh.Action = historyActions::fill;
                        dh.Fill = *args.Fill;
                    }
                    else if (args.Action == historyActions::store_image)
                    {
                        dh.Draw.release();
                        dh.Action = historyActions::store_image;
                        dh.StoredImage = *args.StoreImage;
                    }
                }
            }

            std::list<DrawHistory>::iterator eraseIt = m_history.begin();
            if ((m_hPos - 1) > 0)
            {
                qInfo() << "-- DrawHistory | size before = " << m_history.size();
                std::advance(eraseIt, m_hPos - 1);
                while (eraseIt != m_history.end())
                {
                    if (eraseIt->ID <= m_hPos)
                    {
                        ++eraseIt;
                    }
                    else
                    {
                        eraseIt = m_history.erase(eraseIt);
                    }
                }

                qInfo() << "-- DrawHistory | size after = " << m_history.size();
            }
        }
        // ---------------------------------------------------------------------------

        qInfo() << "-- setBegin() | END true";
        return true;
    }
    m_historyActive = nullptr;
    qInfo() << "-- setBegin() | END false";
    return false;
}

// ================================================================================================================
// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ================================================================================================================

// setActive

// ----------------------------------------------------------------------------------------------------------------

void undo_redo_special::setActive(const UndoRedoSpecialActiveArgs &args)
{
    if (m_historyActive != nullptr)
    {
        ++m_draw_moves;
        m_historyActive->Moves.push_back(*args.Point);
        /*qInfo() << "setActive(args) | point x =" << args.Point->x()
                << ", y = " << args.Point->y()
                << " | moves size = " << m_historyActive->Moves.size();*/
    }
}

// ================================================================================================================
// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ================================================================================================================

// setEnd

// ----------------------------------------------------------------------------------------------------------------

bool undo_redo_special::setEnd(const UndoRedoSpecialEndArgs &args)
{
    bool ok = false;
    if (m_historyActive != nullptr)
    {
        m_historyActive->Area = args.Area;
        if (m_draw_moves >= 0) // m_draw_moves >= m_draw_moves_MAX
        {
            if (m_historyActive->Action == historyActions::draw)
            {
                if (args.Image != nullptr)
                {
                    m_historyActive->StoredImage =
                            args.Image->copy(
                                QRect(args.Area.Left, args.Area.Top, args.Area.Right, args.Area.Bottom)
                                );
                    /*qInfo() << "--- Area | x " << args.Area.Left
                            << ", y " << args.Area.Top
                            << ", w " << args.Area.Right
                            << ", h " << args.Area.Bottom;
                    qInfo() << "--- Image | "
                            << "w " << m_historyActive->StoredImage.size().width()
                            << ", h " << m_historyActive->StoredImage.size().height();*/
                    m_historyActive->DrawCached = true;
                }
            }
        }
        m_draw_moves = 0;

        qInfo() << "setEnd(args) | exit true | id = " << m_historyActive->ID;
        m_historyActive = nullptr;
        ok = true;
    }
    setCache();
    if (!ok)
    {
        qInfo() << "setEnd(args) | exit false" ;
    }
    qInfo() << "--------------------------------------------------------------";
    return ok;
}

// ================================================================================================================
// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ================================================================================================================

bool undo_redo_special::availableRedo()
{
    if (m_hPos < m_hPosMax) return true;
    return false;
}

// ----------------------------------------------------------------------------------------------------------------

bool undo_redo_special::availableUndo()
{
    if (m_hPos > m_hPosStart) return true;
    return false;
}

// ----------------------------------------------------------------------------------------------------------------
