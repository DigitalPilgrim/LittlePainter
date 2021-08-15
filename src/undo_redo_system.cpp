#include "undo_redo_system.h"
#include <QDebug>

// ========================================================================================================

undo_redo_basic undo_redo_system::m_urBasic;
undo_redo_special undo_redo_system::m_urSpecial;

// ========================================================================================================

undo_redo_system::undo_redo_system()
{

}

// --------------------------------------------------------------------------------------------------------

bool undo_redo_system::undo(UndoRedoArgs &ura)
{
    return m_urBasic.undo(ura);
}

// --------------------------------------------------------------------------------------------------------

bool undo_redo_system::redo(UndoRedoArgs &ura)
{
    return m_urBasic.redo(ura);
}

// --------------------------------------------------------------------------------------------------------

bool undo_redo_system::set(const UndoRedoArgs &ura, bool initialize)
{
    return m_urBasic.set(ura, initialize);
}

// --------------------------------------------------------------------------------------------------------

bool undo_redo_system::availableRedo()
{
    return m_urBasic.availableRedo();
}

// --------------------------------------------------------------------------------------------------------

bool undo_redo_system::availableUndo()
{
    return m_urBasic.availableUndo();
}

// --------------------------------------------------------------------------------------------------------

// ========================================================================================================
// ////////////////////////////////////////////////////////////////////////////////////////////////////////
// ========================================================================================================
// SPECIAL
// ========================================================================================================

bool undo_redo_system::undoS(const UndoRedoSpecialArgs &args)
{
    return m_urSpecial.undo(args);
}

bool undo_redo_system::redoS(const UndoRedoSpecialArgs &args)
{
   return m_urSpecial.redo(args);
}

bool undo_redo_system::setS(const UndoRedoSpecialSetIniArgs &args)
{
    return m_urSpecial.setINI(args);
}

bool undo_redo_system::setSBegin(const UndoRedoSpecialBeginArgs &args)
{
     return m_urSpecial.setBegin(args);
}

void undo_redo_system::setSActive(const UndoRedoSpecialActiveArgs &args)
{
    return m_urSpecial.setActive(args);
}

bool undo_redo_system::setSEnd(const UndoRedoSpecialEndArgs &args)
{
    return m_urSpecial.setEnd(args);
}
