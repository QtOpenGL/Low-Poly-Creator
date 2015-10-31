/** *********************************************************************** **/
/** *********************************************************************** **/
/**     Created by:     Ruan (Baggins) Luies 23511354                       **/
/**     email:          23511354@nwu.ac.za                                  **/
/**     Project name:   Virtual Concierge Creator And Management System     **/
/**     File Name:      Node.h                                              **/
/**     From Date:      2015/02/24                                          **/
/**     To Date:        2015/10/01                                          **/
/** **********************************************************************  **/
/** *********************************************************************** **/

#ifndef NODE_H
#define NODE_H
#include <QVector>

class QVector2D;
class QVector3D;
class QString;
class Node
{
public:
    Node();
    Node(QVector3D *);
    Node(QVector3D *, QString *);
    void AddLink(QString *,int); // index from other node
    QVector3D Position();
    QVector3D getColor();
    void setSourceNode();
    void setDestinationNode();
    void setColor(QVector3D *);
    int countConnected();
    int getConnectedIndex(int index);
    QString getLinkedName(int);
    void clearPath();
    void addShortest(int);
    void setG(double g);
    void setShortest(int index);
    int getShortestIndex();
    double getG();
    QString getName();
private:
    QVector3D *m_position;
    QString *m_name;
    QVector3D *m_color;
    QVector<QString*> m_connected;
    QVector<int> m_connectedindex;
    QVector<int> m_shortest;
    int m_nshortest;
    double m_g;
    bool m_links;
};

#endif // NODE_H
