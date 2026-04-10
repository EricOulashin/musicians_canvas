#ifndef EFFECTCHAINDRAGOVERLAY_H
#define EFFECTCHAINDRAGOVERLAY_H

#include <QWidget>

/// Transparent overlay on the effect chain: dashed placeholder rect and blue drop line.
class EffectChainDragOverlay final : public QWidget
{
    Q_OBJECT
public:
    explicit EffectChainDragOverlay(QWidget* parent = nullptr);

    void setPlaceholderRect(const QRect& r);
    void clearPlaceholder();
    void setDropLine(int y, bool visible);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QRect m_placeholder;
    bool  m_hasPlaceholder = false;
    int   m_lineY = 0;
    bool  m_showLine = false;
};

#endif
