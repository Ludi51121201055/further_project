// WindowResizer.cpp
#include "WindowResizer.h"
#include <QApplication>

WindowResizer::WindowResizer(QWidget *targetWindow, QObject *parent)
    : QObject(parent),
      m_targetWindow(targetWindow),
      m_resizing(false),
      m_resizeEdge(0),
      m_edgeThreshold(8)
{
    // 安装事件过滤器到目标窗口
    m_targetWindow->installEventFilter(this);
    // 确保窗口可以调整大小
    m_targetWindow->setMinimumSize(100, 100);
}

WindowResizer::~WindowResizer()
{
}

bool WindowResizer::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == m_targetWindow) {
        switch (event->type()) {
        case QEvent::MouseMove: {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            if (m_resizing) {
                // 调整窗口大小
                QRect geometry = m_targetWindow->frameGeometry();
                QPoint delta = mouseEvent->globalPos() - m_lastPos;

                if (m_resizeEdge & Qt::LeftEdge) {
                    int newLeft = geometry.left() + delta.x();
                    if (newLeft < geometry.right() - m_targetWindow->minimumWidth())
                        geometry.setLeft(newLeft);
                } else if (m_resizeEdge & Qt::RightEdge) {
                    int newRight = geometry.right() + delta.x();
                    if (newRight > geometry.left() + m_targetWindow->minimumWidth())
                        geometry.setRight(newRight);
                }

                if (m_resizeEdge & Qt::TopEdge) {
                    int newTop = geometry.top() + delta.y();
                    if (newTop < geometry.bottom() - m_targetWindow->minimumHeight())
                        geometry.setTop(newTop);
                } else if (m_resizeEdge & Qt::BottomEdge) {
                    int newBottom = geometry.bottom() + delta.y();
                    if (newBottom > geometry.top() + m_targetWindow->minimumHeight())
                        geometry.setBottom(newBottom);
                }

                m_targetWindow->setGeometry(geometry);
                m_lastPos = mouseEvent->globalPos();
                return true;
            } else {
                // 更新光标样式
                Qt::Edges edges = getEdges(mouseEvent->pos());
                if (edges == (Qt::LeftEdge | Qt::TopEdge) || edges == (Qt::RightEdge | Qt::BottomEdge))
                    m_targetWindow->setCursor(Qt::SizeFDiagCursor);
                else if (edges == (Qt::RightEdge | Qt::TopEdge) || edges == (Qt::LeftEdge | Qt::BottomEdge))
                    m_targetWindow->setCursor(Qt::SizeBDiagCursor);
                else if (edges == Qt::LeftEdge || edges == Qt::RightEdge)
                    m_targetWindow->setCursor(Qt::SizeHorCursor);
                else if (edges == Qt::TopEdge || edges == Qt::BottomEdge)
                    m_targetWindow->setCursor(Qt::SizeVerCursor);
                else
                    m_targetWindow->setCursor(Qt::ArrowCursor);
            }
            break;
        }
        case QEvent::MouseButtonPress: {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            if (mouseEvent->button() == Qt::LeftButton) {
                m_resizeEdge = getEdges(mouseEvent->pos());
                m_resizing = (m_resizeEdge != 0);
                m_lastPos = mouseEvent->globalPos();
                if (m_resizing)
                    return true;
            }
            break;
        }
        case QEvent::MouseButtonRelease: {
            if (static_cast<QMouseEvent*>(event)->button() == Qt::LeftButton) {
                m_resizing = false;
                m_resizeEdge = 0;
            }
            break;
        }
        case QEvent::Leave:
            // 鼠标离开窗口时恢复默认光标
            m_targetWindow->setCursor(Qt::ArrowCursor);
            break;
        default:
            break;
        }
    }

    return QObject::eventFilter(obj, event);
}

Qt::Edges WindowResizer::getEdges(const QPoint &pos) const
{
    Qt::Edges edges = 0;
    QRect rect = m_targetWindow->rect();

    if (pos.x() <= m_edgeThreshold)
        edges |= Qt::LeftEdge;
    else if (pos.x() >= rect.width() - m_edgeThreshold)
        edges |= Qt::RightEdge;

    if (pos.y() <= m_edgeThreshold)
        edges |= Qt::TopEdge;
    else if (pos.y() >= rect.height() - m_edgeThreshold)
        edges |= Qt::BottomEdge;

    return edges;
}
