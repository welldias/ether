#ifndef __ETHER_STATISTICS_H__
#define __ETHER_STATISTICS_H__

struct _Ether_Statistics
{
	int objects_processed;
	int objects_not_invisible;
	int objects_not_behind;
	int objects_not_leftright;
	int objects_not_abovebelow;
	int objects_not_toosmall;
	int lights_processed;
	int facets_processed;
	int facets_not_backfacing;
	int facets_flatshaded;
	int vertices_zchecked;
	int vertices_ztransformed;
	int facets_not_behind;
	int vertices_xychecked;
	int vertices_xytransformed;
	int vertices_projected_regular;
	int facets_gouraud_shaded;
	int vertex_pointlights;
	int vertex_dirlights;
	int detail_facets;
	int not_toomanyfacets;
	int facets_need_x_clipping;
	int facets_need_y_clipping;
	int facets_need_z_clipping;
};

EAPI void                 ether_statistics_clear  (void);
EAPI EtherStatistics    *ether_statistics_get    (void);

#endif /* __ETHER_STATISTICS_H__ */
