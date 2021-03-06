#ifndef UNDO_REDO_SYSTEM_H
#define UNDO_REDO_SYSTEM_H

#include <QImage>
#include <QSize>

#include <list>
#include "floattexture.h"
#include "graphic_helper_functions.h"
#include "undo_redo_basic.h"
#include "undo_redo_special.h"

// =================================================================================

/* =================================================================================
 * /////////////////////////////////////////////////////////////////////////////////
 * ---------------------------------------------------------------------------------
 * UNDO REDO
 * ---------
 * =================================================================================
 * */

class undo_redo_system
{
    static undo_redo_basic m_urBasic;
    static undo_redo_special m_urSpecial;
    undo_redo_system();
public:

    static void setFileCachePath(const QString& path);

    // BASIC
    // zakladny undo redo NEPOZIVA SA
    static bool undo(UndoRedoArgs & ura);
    static bool redo(UndoRedoArgs & ura);
    static bool set(const UndoRedoArgs & ura, bool initialize = false);
    static bool availableRedo();
    static bool availableUndo();

    // SPECIAL
    // undo redo s ukladanim tahov, casti tahov ako obrazok a ukladanie do suborov
    static bool undoS(UndoRedoSpecialArgs& args);
    static bool redoS(UndoRedoSpecialArgs& args);

    static bool setS(const UndoRedoSpecialSetIniArgs &args);
    static bool setSBegin(const UndoRedoSpecialBeginArgs &args);
    static void setSActive(const UndoRedoSpecialActiveArgs &args);
    static bool setSEnd(const UndoRedoSpecialEndArgs &args);
};

#endif // UNDO_REDO_SYSTEM_H
