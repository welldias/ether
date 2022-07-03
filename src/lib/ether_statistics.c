#include "ether_statistics.h"

static EtherStatistics _ether_stats;

EtherStatistics *
ether_statistics_get(void)
{
	return &_ether_stats;
}

void 
ether_statistics_clear(void)
{
	_ether_stats.objects_processed = 0;
	_ether_stats.objects_not_invisible = 0;
	_ether_stats.objects_not_behind = 0;
	_ether_stats.objects_not_leftright = 0;
	_ether_stats.objects_not_abovebelow = 0;
	_ether_stats.objects_not_toosmall = 0;
	_ether_stats.lights_processed = 0;
	_ether_stats.facets_processed = 0;
	_ether_stats.facets_not_backfacing = 0;
	_ether_stats.facets_flatshaded = 0;
	_ether_stats.vertices_zchecked = 0;
	_ether_stats.vertices_ztransformed = 0;
	_ether_stats.facets_not_behind = 0;
	_ether_stats.vertices_xychecked = 0;
	_ether_stats.vertices_xytransformed = 0;
	_ether_stats.vertices_projected_regular = 0;
	_ether_stats.facets_gouraud_shaded = 0;
	_ether_stats.vertex_pointlights = 0;
	_ether_stats.vertex_dirlights = 0;
	_ether_stats.detail_facets = 0;
	_ether_stats.not_toomanyfacets = 0;
	_ether_stats.facets_need_x_clipping = 0;
	_ether_stats.facets_need_y_clipping = 0;
	_ether_stats.facets_need_z_clipping = 0;
}
