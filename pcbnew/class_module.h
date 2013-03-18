/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2004 Jean-Pierre Charras, jaen-pierre.charras@gipsa-lab.inpg.com
 * Copyright (C) 1992-2011 KiCad Developers, see AUTHORS.txt for contributors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, you may find one here:
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
 * or you may search the http://www.gnu.org website for the version 2 license,
 * or you may write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */

/**
 * @file class_module.h
 * @brief Module description (excepted pads)
 */


#ifndef MODULE_H_
#define MODULE_H_


#include <dlist.h>
#include <layers_id_colors_and_visibility.h>       // ALL_LAYERS definition.
#include <class_board_item.h>

#include <class_text_mod.h>
#include "zones.h"

class LINE_READER;
class EDA_3D_CANVAS;
class S3D_MASTER;
class EDA_DRAW_PANEL;
class D_PAD;
class BOARD;
class MSG_PANEL_ITEM;


/**
 * Enum MODULE_ATTR_T
 * is the set of attributes allowed within a MODULE, using MODULE::SetAttributes()
 * and MODULE::GetAttributes().  These are to be ORed together when calling
 * MODULE::SetAttributes()
 */
enum MODULE_ATTR_T
{
    MOD_DEFAULT = 0,    ///< default
    MOD_CMS     = 1,    ///< Set for modules listed in the automatic insertion list
                        ///< (usually SMD footprints)
    MOD_VIRTUAL = 2     ///< Virtual component: when created by copper shapes on
                        ///<  board (Like edge card connectors, mounting hole...)
};


class MODULE : public BOARD_ITEM
{
public:
    MODULE( BOARD* parent );

    MODULE( const MODULE& aModule );

    ~MODULE();

    MODULE* Next() const { return (MODULE*) Pnext; }
    MODULE* Back() const { return (MODULE*) Pback; }

    void Copy( MODULE* Module );        // Copy structure

    /*
     * Function Add
     * adds the given item to this MODULE and takes ownership of its memory.
     * @param aBoardItem The item to add to this board.
     * @param doInsert If true, then insert, else append
     *  void    Add( BOARD_ITEM* aBoardItem, bool doInsert = true );
     */

    /**
     * Function CalculateBoundingBox
     * calculates the bounding box in board coordinates.
     */
    void CalculateBoundingBox();

    /**
     * Function GetFootPrintRect()
     * Returns the area of the module footprint excluding any text.
     * @return EDA_RECT - The rectangle containing the footprint.
     */
    EDA_RECT GetFootPrintRect() const;

    EDA_RECT GetBoundingBox() const;

    DLIST<D_PAD>& Pads()                        { return m_Pads; }
    const DLIST<D_PAD>& Pads() const            { return m_Pads; }

    DLIST<BOARD_ITEM>& GraphicalItems()         { return m_Drawings; }
    const DLIST<BOARD_ITEM>& GraphicalItems() const { return m_Drawings; }

    DLIST<S3D_MASTER>& Models()                 { return m_3D_Drawings; }
    const DLIST<S3D_MASTER>& Models() const     { return m_3D_Drawings; }

    void SetPosition( const wxPoint& aPos );                        // was overload
    const wxPoint& GetPosition() const          { return m_Pos; }   // was overload

    void SetOrientation( double newangle );
    double GetOrientation() const { return m_Orient; }

    const wxString& GetLibRef() const { return m_LibRef; }
    void SetLibRef( const wxString& aLibRef ) { m_LibRef = aLibRef; }

    const wxString& GetDescription() const { return m_Doc; }
    void SetDescription( const wxString& aDoc ) { m_Doc = aDoc; }

    const wxString& GetKeywords() const { return m_KeyWord; }
    void SetKeywords( const wxString& aKeywords ) { m_KeyWord = aKeywords; }

    const wxString& GetPath() const { return m_Path; }
    void SetPath( const wxString& aPath ) { m_Path = aPath; }

    int GetLocalSolderMaskMargin() const { return m_LocalSolderMaskMargin; }
    void SetLocalSolderMaskMargin( int aMargin ) { m_LocalSolderMaskMargin = aMargin; }

    int GetLocalClearance() const { return m_LocalClearance; }
    void SetLocalClearance( int aClearance ) { m_LocalClearance = aClearance; }

    int GetLocalSolderPasteMargin() const { return m_LocalSolderPasteMargin; }
    void SetLocalSolderPasteMargin( int aMargin ) { m_LocalSolderPasteMargin = aMargin; }

    double GetLocalSolderPasteMarginRatio() const { return m_LocalSolderPasteMarginRatio; }
    void SetLocalSolderPasteMarginRatio( double aRatio ) { m_LocalSolderPasteMarginRatio = aRatio; }

    void SetZoneConnection( ZoneConnection aType ) { m_ZoneConnection = aType; }
    ZoneConnection GetZoneConnection() const { return m_ZoneConnection; }

    void SetThermalWidth( int aWidth ) { m_ThermalWidth = aWidth; }
    int GetThermalWidth() const { return m_ThermalWidth; }

    void SetThermalGap( int aGap ) { m_ThermalGap = aGap; }
    int GetThermalGap() const { return m_ThermalGap; }

    int GetAttributes() const { return m_Attributs; }
    void SetAttributes( int aAttributes ) { m_Attributs = aAttributes; }

    void SetFlag( int aFlag ) { flag = aFlag; }
    void IncrementFlag() { flag += 1; }
    int GetFlag() const { return flag; }

    void Move( const wxPoint& aMoveVector );

    void Rotate( const wxPoint& aRotCentre, double aAngle );

    void Flip( const wxPoint& aCentre );

    /**
     * function IsFlipped
     * @return true if the module is flipped, i.e. on the back side of the board
     */
    bool IsFlipped() const {return GetLayer() == LAYER_N_BACK; }

// m_ModuleStatus bits:
#define MODULE_is_LOCKED    0x01        ///< module LOCKED: no autoplace allowed
#define MODULE_is_PLACED    0x02        ///< In autoplace: module automatically placed
#define MODULE_to_PLACE     0x04        ///< In autoplace: module waiting for autoplace


    bool IsLocked() const
    {
        return (m_ModuleStatus & MODULE_is_LOCKED) != 0;
    }

    /**
     * Function SetLocked
     * sets the MODULE_is_LOCKED bit in the m_ModuleStatus
     * @param isLocked When true means turn on locked status, else unlock
     */
    void SetLocked( bool isLocked )
    {
        if( isLocked )
            m_ModuleStatus |= MODULE_is_LOCKED;
        else
            m_ModuleStatus &= ~MODULE_is_LOCKED;
    }

    bool IsPlaced() const   { return (m_ModuleStatus & MODULE_is_PLACED); }
    void SetIsPlaced( bool isPlaced )
    {
        if( isPlaced )
            m_ModuleStatus |= MODULE_is_PLACED;
        else
            m_ModuleStatus &= ~MODULE_is_PLACED;
    }

    bool NeedsPlaced() const  { return (m_ModuleStatus & MODULE_to_PLACE); }
    void SetNeedsPlaced( bool needsPlaced )
    {
        if( needsPlaced )
            m_ModuleStatus |= MODULE_to_PLACE;
        else
            m_ModuleStatus &= ~MODULE_to_PLACE;
    }

    void SetLastEditTime( time_t aTime ) { m_LastEditTime = aTime; }
    void SetLastEditTime( ) { m_LastEditTime = time( NULL ); }
    time_t GetLastEditTime() const { return m_LastEditTime; }

    /* drawing functions */

    void Draw( EDA_DRAW_PANEL* aPanel,
               wxDC*           aDC,
               GR_DRAWMODE     aDrawMode,
               const wxPoint&  aOffset = ZeroOffset );

    void Draw3D( EDA_3D_CANVAS* glcanvas );

    void DrawEdgesOnly( EDA_DRAW_PANEL* panel, wxDC* DC, const wxPoint& offset,
                        GR_DRAWMODE draw_mode );

    void DrawAncre( EDA_DRAW_PANEL* panel, wxDC* DC,
                    const wxPoint& offset, int dim_ancre, GR_DRAWMODE draw_mode );

    void GetMsgPanelInfo( std::vector< MSG_PANEL_ITEM >& aList );

    bool HitTest( const wxPoint& aPosition );

    bool HitTest( const EDA_RECT& aRect ) const;

    /**
     * Function GetReference
     * @return const wxString& - the reference designator text.
     */
    const wxString& GetReference() const
    {
        return m_Reference->m_Text;
    }

    /**
     * Function SetReference
     * @param aReference A reference to a wxString object containing the reference designator
     *                   text.
     */
    void SetReference( const wxString& aReference )
    {
        m_Reference->m_Text = aReference;
    }

    /**
     * Function GetValue
     * @return const wxString& - the value text.
     */
    const wxString& GetValue()
    {
        return m_Value->m_Text;
    }

    /**
     * Function SetValue
     * @param aValue A reference to a wxString object containing the value text.
     */
    void SetValue( const wxString& aValue )
    {
        m_Value->m_Text = aValue;
    }

    /// read/write accessors:
    TEXTE_MODULE& Value()       { return *m_Value; }
    TEXTE_MODULE& Reference()   { return *m_Reference; }

    /// The const versions to keep the compiler happy.
    TEXTE_MODULE& Value() const       { return *m_Value; }
    TEXTE_MODULE& Reference() const   { return *m_Reference; }


    /**
     * Function FindPadByName
     * returns a D_PAD* with a matching name.  Note that names may not be
     * unique, depending on how the foot print was created.
     * @param aPadName the pad name to find
     * @return D_PAD* - The first matching name is returned, or NULL if not
     *                  found.
     */
    D_PAD* FindPadByName( const wxString& aPadName ) const;

    /**
     * Function GetPad
     * get a pad at \a aPosition on \a aLayer in the footprint.
     *
     * @param aPosition A wxPoint object containing the position to hit test.
     * @param aLayerMask A layer or layers to mask the hit test.
     * @return A pointer to a D_PAD object if found otherwise NULL.
     */
    D_PAD* GetPad( const wxPoint& aPosition, int aLayerMask = ALL_LAYERS );

    /**
     * GetPadCount
     * returns the number of pads.
     */
    unsigned GetPadCount() const            { return m_Pads.GetCount() ; }

    double GetArea() const                  { return m_Surface; }

    time_t GetLink() const                  { return m_Link; }
    void SetLink( time_t aLink )            { m_Link = aLink; }

    int GetPlacementCost180() const         { return m_CntRot180; }
    void SetPlacementCost180( int aCost )   { m_CntRot180 = aCost; }

    int GetPlacementCost90() const          { return m_CntRot90; }
    void SetPlacementCost90( int aCost )    { m_CntRot90 = aCost; }

    /**
     * Function Add3DModel
     * adds \a a3DModel definition to the end of the 3D model list.
     *
     * @param a3DModel A pointer to a #S3D_MASTER to add to the list.
     */
    void Add3DModel( S3D_MASTER* a3DModel );

    /**
     * Function AddPad
     * adds \a aPad to the end of the pad list.
     *
     * @param aPad A pointer to a #D_PAD to add to the list.
     */
    void AddPad( D_PAD* aPad );

    SEARCH_RESULT Visit( INSPECTOR* inspector, const void* testData,
                         const KICAD_T scanTypes[] );

    wxString GetClass() const
    {
        return wxT( "MODULE" );
    }

    wxString GetSelectMenuText() const;

    BITMAP_DEF GetMenuImage() const { return  module_xpm; }

    EDA_ITEM* Clone() const;

    /**
     * static function IsLibNameValid
     * Test for validity of a name of a footprint to be used in a footprint library
     * ( no spaces, dir separators ... )
     * @param aName = the name in library to validate
     * @return true if the given name is valid
     */
    static bool IsLibNameValid( const wxString & aName );

    /**
     * static function ReturnStringLibNameInvalidChars
     * Test for validity of the name in a library of the footprint
     * ( no spaces, dir separators ... )
     * @param aUserReadable = false to get the list of invalid chars
     *        true to get a readable form (i.e ' ' = 'space' '\\t'= 'tab')
     * @return a constant std::string giving the list of invalid chars in lib name
     */
    static const wxChar* ReturnStringLibNameInvalidChars( bool aUserReadable );

#if defined(DEBUG)
    void Show( int nestLevel, std::ostream& os ) const;     // overload
#endif

private:
    DLIST<D_PAD>      m_Pads;           ///< Linked list of pads.
    DLIST<BOARD_ITEM> m_Drawings;       ///< Linked list of graphical items.
    DLIST<S3D_MASTER> m_3D_Drawings;    ///< Linked list of 3D models.
    double            m_Orient;         ///< Orientation in tenths of a degree, 900=90.0 degrees.
    wxPoint           m_Pos;            ///< Position of module on the board in internal units.
    TEXTE_MODULE*     m_Reference;      ///< Component reference designator value (U34, R18..)
    TEXTE_MODULE*     m_Value;          ///< Component value (74LS00, 22K..)
    wxString          m_LibRef;         ///< Name of the module in the library.
    int               m_Attributs;      ///< Flag bits ( see Mod_Attribut )
    int               m_ModuleStatus;   ///< For autoplace: flags (LOCKED, AUTOPLACED)
    EDA_RECT          m_BoundaryBox;    ///< Bounding box : coordinates on board, real orientation.

    // The final margin is the sum of these 2 values
    int               m_ThermalWidth;
    int               m_ThermalGap;
    wxString          m_Doc;            ///< File name and path for documentation file.
    wxString          m_KeyWord;        ///< Search keywords to find module in library.
    wxString          m_Path;
    ZoneConnection    m_ZoneConnection;
    time_t            m_LastEditTime;
    int               flag;             ///< Use to trace ratsnest and auto routing.
    double            m_Surface;        ///< Bounding box area
    time_t            m_Link;           ///< Temporary logical link used in edition
    int               m_CntRot90;       ///< Horizontal automatic placement cost ( 0..10 ).
    int               m_CntRot180;      ///< Vertical automatic placement cost ( 0..10 ).

    // Local tolerances. When zero, this means the corresponding netclass value
    // is used. Usually theses local tolerances zero, in deference to the
    // corresponding netclass values.
    int               m_LocalClearance;
    int               m_LocalSolderMaskMargin;    ///< Solder mask margin
    int               m_LocalSolderPasteMargin;   ///< Solder paste margin absolute value
    double            m_LocalSolderPasteMarginRatio;   ///< Solder mask margin ratio
                                                       ///< value of pad size
};

#endif     // MODULE_H_
