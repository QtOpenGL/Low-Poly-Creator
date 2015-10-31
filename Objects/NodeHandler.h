/** *********************************************************************** **/
/** *********************************************************************** **/
/**     Created by:     Ruan (Baggins) Luies 23511354                       **/
/**     email:          23511354@nwu.ac.za                                  **/
/**     Project name:   Virtual Concierge Creator And Management System     **/
/**     File Name:      NodeHandler.h                                       **/
/**     From Date:      2015/02/24                                          **/
/**     To Date:        2015/10/01                                          **/
/** **********************************************************************  **/
/** *********************************************************************** **/

#ifndef NODEHANDLER_H
#define NODEHANDLER_H
#include "VCCMObjects/Node.h"
#include <QVector>
#include <QString>
#include <qdebug.h>
class QVector3D;
class NodeHandler
{
public:
    NodeHandler();
    void AddNode(Node*);
    Node NodeFromIndex(unsigned int);
    void AddNodeLink(int,QString*);
    void AddNodeLinkbyIndex(int,int);
    void CalculateShortest(int,int);
    void ReadFilePVC(QString);
    int count();
    int pathcount();
    int pathindex(int);
private:
    QVector<Node*> m_premises;
    QVector<int> m_shortest;

};

#endif // NODEHANDLER_H
