#ifndef _LOGINFOHEADERVIEW_H
#define _LOGINFOHEADERVIEW_H

#include <QHeaderView>
#include <QComboBox>
#include <QTableWidgetItem>

class LogInfoHeaderView : public QHeaderView
{
	Q_OBJECT
public:
	LogInfoHeaderView(int checkColumnIndex,
		Qt::Orientation orientation,
		QWidget * parent = 0) :
		QHeaderView(orientation, parent)
	{
		nwLayerfilter = new QComboBox(this);
		nwLayerfilter->addItem("全部网络层");
		nwLayerfilter->addItem("IPv4");
		nwLayerfilter->addItem("IPv6");
		nwLayerfilter->addItem("ARP");
		nwLayerfilter->addItem("ICMP");
		nwLayerfilter->addItem("ICMPv6");
		nwLayerfilter->addItem("Other");
		nwLayerfilter->setCurrentIndex(nwLayerfilter->findText("全部网络层"));

		tpLayerfilter = new QComboBox(this);
		tpLayerfilter->addItem("全部传输层");
		tpLayerfilter->addItem("TCP");
		tpLayerfilter->addItem("UDP");
		tpLayerfilter->addItem("Other");
		tpLayerfilter->setCurrentIndex(tpLayerfilter->findText("全部传输层"));

	}

	QString getNetworkFilter()
	{
		return nwLayerfilter->currentText();
	}

	QString getTransportFilter()
	{
		return tpLayerfilter->currentText();
	}

	QComboBox * nwLayerfilter;
	QComboBox * appLayerfilter;
	QComboBox * tpLayerfilter;
protected:
    void paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const
    {
		switch (logicalIndex)
		{
		case 1:
			nwLayerfilter->setGeometry(rect);
			break;
		case 2:
			tpLayerfilter->setGeometry(rect);
			break;
		default:
			QHeaderView::paintSection(painter, rect, logicalIndex);
		}
    }
};
#endif