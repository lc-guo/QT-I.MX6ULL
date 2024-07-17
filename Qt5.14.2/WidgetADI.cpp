#ifndef WIDGETADI_CPP
#define WIDGETADI_CPP
#endif

////////////////////////////////////////////////////////////////////////////////

#include "WidgetADI.h"
#include "ui_WidgetADI.h"

////////////////////////////////////////////////////////////////////////////////

WidgetADI::WidgetADI( QWidget *parent ) :
    QWidget( parent ),
    m_ui( new Ui::WidgetADI ),
    m_adi ( 0 ),
    m_layoutSq ( 0 )
{
    m_ui->setupUi( this );

    setupUi();

    m_adi = m_ui->graphicsADI;
}

////////////////////////////////////////////////////////////////////////////////

WidgetADI::~WidgetADI()
{
    if ( m_layoutSq ) delete m_layoutSq; m_layoutSq = 0;

    if ( m_ui ) delete m_ui; m_ui = 0;
}

////////////////////////////////////////////////////////////////////////////////

void WidgetADI::setupUi()
{
    m_layoutSq = new LayoutSquare( this );

    m_layoutSq->setContentsMargins( 0, 0, 0, 0 );
    m_layoutSq->addWidget( m_ui->graphicsADI );

    setLayout( m_layoutSq );
}
