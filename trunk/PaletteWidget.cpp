#include "PaletteWidget.h"

#include <iostream>
#include <QPainter>
#include <QPaintEvent>
#include <QColorDialog>

QSize PaletteWidget::minimumSizeHint() const
{
    return QSize(100, 100);
}


QSize PaletteWidget::sizeHint() const
{
    return QSize(100, 200);
}


QColor PaletteWidget::toQColor(const Imath::Color4f& in)
{
    return QColor((int)(in.r * 255.0f),
                  (int)(in.g * 255.0f),
                  (int)(in.b * 255.0f),
                  (int)(in.a * 255.0f));
}


void PaletteWidget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);

    // Some useful colors|tools
    Imath::Color4f brighterBackground = m_backgroundColor * 1.05; 
    brighterBackground.a = 1.0f;
    QPen brighterBackgroundPen = QPen(toQColor(brighterBackground));

    
    // BG clear
    painter.fillRect(0, 0, width(), height(), QBrush(toQColor(m_backgroundColor)));
    
    // Active and passive colors get drawn in the upper-left
    painter.fillRect(40, 30, 28, 28, QBrush(QColor(0,0,0)));
    painter.fillRect(41, 31, 26, 26, QBrush(QColor(255, 255, 255)));
    painter.fillRect(42, 32, 24, 24, QBrush(toQColor(m_passiveColor)));

    painter.fillRect(28, 18, 28, 28, QBrush(QColor(0,0,0)));
    painter.fillRect(29, 19, 26, 26, QBrush(QColor(255, 255, 255)));
    painter.fillRect(30, 20, 24, 24, QBrush(toQColor(m_activeColor)));
    
    // Horizontal rule
    painter.setPen(brighterBackgroundPen);
    painter.drawLine(QPoint(0, 75), QPoint(width(), 75));
}


void PaletteWidget::mousePressEvent(QMouseEvent* event)
{
    QColor color;
    
    switch(clickHit(event->pos()))
    {
        // TODO: Why can i not add the additional two parameters (to get alpha) to the color dialog?
        case HIT_ACTIVE_COLOR_BOX:
            color = QColorDialog::getColor(QColor((int)(m_activeColor.r * 255.0f),
                                                  (int)(m_activeColor.g * 255.0f),
                                                  (int)(m_activeColor.b * 255.0f),
                                                  (int)(m_activeColor.a * 255.0f)), this);
            setActiveColor(Imath::Color4f((float)color.red()/255.0f,
                                          (float)color.green()/255.0f,
                                          (float)color.blue()/255.0f,
                                          (float)color.alpha()/255.0f));
            break;
        case HIT_PASSIVE_COLOR_BOX:
            color = QColorDialog::getColor(QColor((int)(m_passiveColor.r * 255.0f),
                                                  (int)(m_passiveColor.g * 255.0f),
                                                  (int)(m_passiveColor.b * 255.0f),
                                                  (int)(m_passiveColor.a * 255.0f)), this);
            setPassiveColor(Imath::Color4f((float)color.red()/255.0f,
                                           (float)color.green()/255.0f,
                                           (float)color.blue()/255.0f,
                                           (float)color.alpha()/255.0f));
            break;
        default: 
            break;
    }
}


void PaletteWidget::mouseDoubleClickEvent(QMouseEvent* event)
{
    // Emit palette changed signal
    // emit activeColorChanged(Imath::Color4f(1.0f, 0.0f, 1.0f, 1.0f));

    std::cout << event->pos().x() << " " << event->pos().y() << std::endl;
    std::cout << clickHit(event->pos()) << std::endl;
}


PaletteWidget::HitType PaletteWidget::clickHit(const QPoint& clickSpot)
{
    if (clickSpot.x() > 30 && clickSpot.y() > 20 && clickSpot.x() < 54 && clickSpot.y() < 44)
        return HIT_ACTIVE_COLOR_BOX;
    else if (clickSpot.x() > 42 && clickSpot.y() > 32 && clickSpot.x() < 66 && clickSpot.y() < 56)
        return HIT_PASSIVE_COLOR_BOX;
    
    return HIT_NONE;
}


void PaletteWidget::setActiveColor(const Imath::Color4f& color)
{
    m_activeColor = color;
    emit activeColorChanged(m_activeColor);
    repaint(0, 0, width(), 75);
}


void PaletteWidget::setPassiveColor(const Imath::Color4f& color)
{
    m_passiveColor = color;
    emit activeColorChanged(m_activeColor);
    repaint(0, 0, width(), 75);
}


void PaletteWidget::swapColors()
{
    Imath::Color4f copy = m_activeColor;
    m_activeColor = m_passiveColor;
    m_passiveColor = copy;
    emit activeColorChanged(m_activeColor);
    repaint(0, 0, width(), 75);
}