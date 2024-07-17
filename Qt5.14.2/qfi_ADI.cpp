#ifndef QFI_ADI_CPP
#define QFI_ADI_CPP
#endif

////////////////////////////////////////////////////////////////////////////////

#ifdef WIN32
#   include <float.h>
#endif

#include <math.h>

#include "qfi_ADI.h"

////////////////////////////////////////////////////////////////////////////////

qfi_ADI::qfi_ADI( QWidget *parent ) :
    QGraphicsView ( parent ),

    m_scene ( nullptr ),

    m_itemBack ( nullptr ),
    m_itemFace ( nullptr ),
    m_itemRing ( nullptr ),
    m_itemCase ( nullptr ),

    m_roll  ( 0.0f ),
    m_pitch ( 0.0f ),

    m_faceDeltaX_new ( 0.0f ),
    m_faceDeltaX_old ( 0.0f ),
    m_faceDeltaY_new ( 0.0f ),
    m_faceDeltaY_old ( 0.0f ),

    m_scaleX ( 1.0f ),
    m_scaleY ( 1.0f ),

    m_originalHeight ( 240 ),
    m_originalWidth  ( 240 ),

    m_originalPixPerDeg ( 1.7f ),

    m_originalAdiCtr ( 120.0f , 120.0f ),

    m_backZ ( -30 ),
    m_faceZ ( -20 ),
    m_ringZ ( -10 ),
    m_caseZ (  10 )
{
    reset();

    m_scene = new QGraphicsScene( this );
    setScene( m_scene );

    m_scene->clear();

    init();
}

////////////////////////////////////////////////////////////////////////////////

qfi_ADI::~qfi_ADI()
{
    if ( m_scene != nullptr )
    {
        m_scene->clear();
        delete m_scene;
        m_scene = nullptr;
    }

    reset();
}

////////////////////////////////////////////////////////////////////////////////

void qfi_ADI::reinit()
{
    if ( m_scene )
    {
        m_scene->clear();

        init();
    }
}

////////////////////////////////////////////////////////////////////////////////

void qfi_ADI::update()
{
    updateView();

    m_faceDeltaX_old  = m_faceDeltaX_new;
    m_faceDeltaY_old  = m_faceDeltaY_new;
}

////////////////////////////////////////////////////////////////////////////////

void qfi_ADI::setRoll( float roll )
{
    m_roll = roll;

    if ( m_roll < -180.0f ) m_roll = -180.0f;
    if ( m_roll >  180.0f ) m_roll =  180.0f;
}

////////////////////////////////////////////////////////////////////////////////

void qfi_ADI::setPitch( float pitch )
{
    m_pitch = pitch;

    if ( m_pitch < -25.0f ) m_pitch = -25.0f;
    if ( m_pitch >  25.0f ) m_pitch =  25.0f;
}

////////////////////////////////////////////////////////////////////////////////

void qfi_ADI::resizeEvent( QResizeEvent *event )
{
    ////////////////////////////////////
    QGraphicsView::resizeEvent( event );
    ////////////////////////////////////

    reinit();
}

////////////////////////////////////////////////////////////////////////////////

void qfi_ADI::init()
{
    m_scaleX = (float)width()  / (float)m_originalWidth;
    m_scaleY = (float)height() / (float)m_originalHeight;

    reset();

    m_itemBack = new QGraphicsSvgItem( ":/images/adi_back.svg" );
    m_itemBack->setCacheMode( QGraphicsItem::NoCache );
    m_itemBack->setZValue( m_backZ );
    m_itemBack->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_itemBack->setTransformOriginPoint( m_originalAdiCtr );
    m_scene->addItem( m_itemBack );

    m_itemFace = new QGraphicsSvgItem( ":/images/adi_face.svg" );
    m_itemFace->setCacheMode( QGraphicsItem::NoCache );
    m_itemFace->setZValue( m_faceZ );
    m_itemFace->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_itemFace->setTransformOriginPoint( m_originalAdiCtr );
    m_scene->addItem( m_itemFace );

    m_itemRing = new QGraphicsSvgItem( ":/images/adi_ring.svg" );
    m_itemRing->setCacheMode( QGraphicsItem::NoCache );
    m_itemRing->setZValue( m_ringZ );
    m_itemRing->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_itemRing->setTransformOriginPoint( m_originalAdiCtr );
    m_scene->addItem( m_itemRing );

    m_itemCase = new QGraphicsSvgItem( ":/images/adi_case.svg" );
    m_itemCase->setCacheMode( QGraphicsItem::NoCache );
    m_itemCase->setZValue( m_caseZ );
    m_itemCase->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_scene->addItem( m_itemCase );

    centerOn( width() / 2.0f , height() / 2.0f );

    updateView();
}

////////////////////////////////////////////////////////////////////////////////

void qfi_ADI::reset()
{
    m_itemBack = nullptr;
    m_itemFace = nullptr;
    m_itemRing = nullptr;
    m_itemCase = nullptr;

    m_roll  = 0.0f;
    m_pitch = 0.0f;

    m_faceDeltaX_new = 0.0f;
    m_faceDeltaX_old = 0.0f;
    m_faceDeltaY_new = 0.0f;
    m_faceDeltaY_old = 0.0f;
}

////////////////////////////////////////////////////////////////////////////////

void qfi_ADI::updateView()
{
    m_scaleX = (float)width()  / (float)m_originalWidth;
    m_scaleY = (float)height() / (float)m_originalHeight;

    m_itemBack->setRotation( - m_roll );
    m_itemFace->setRotation( - m_roll );
    m_itemRing->setRotation( - m_roll );

    float roll_rad = M_PI * m_roll / 180.0;

    float delta  = m_originalPixPerDeg * m_pitch;

    m_faceDeltaX_new = m_scaleX * delta * sin( roll_rad );
    m_faceDeltaY_new = m_scaleY * delta * cos( roll_rad );

    m_itemFace->moveBy( m_faceDeltaX_new - m_faceDeltaX_old, m_faceDeltaY_new - m_faceDeltaY_old );

    m_scene->update();
}
