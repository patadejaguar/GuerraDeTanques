TEMPLATE = subdirs

SUBDIRS += \
        ../xav \
        gt_core \
        gt_plugins \
        gt_build_terrain \
        gt_client \
        gt_server
	

android {
QT += androidextras
}
