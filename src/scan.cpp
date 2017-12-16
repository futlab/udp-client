#include "scan.h"
#include <Eigen/Core>
#include <pcl/point_types.h>
#include <pcl/io/pcd_io.h>


Scan::Scan(QObject *parent) : QObject(parent)
{

}

void Scan::loadPCD(const QString &fileName)
{
    //std::istream &istream;
    //pcl::PCDReader reader;
    pcl::PointCloud<pcl::PointXYZ> cloud;
    if(pcl::io::loadPCDFile<pcl::PointXYZ>(fileName.toStdString(), cloud)) {
        return;
    }

    int x = 5 + 8;
    return;

}
