#include <Ether.h>

static EtherStatistics _Ether_Stats;

EtherStatistics *
ether_statistics_get(void)
{
	return &_Ether_Stats;
}

void 
ether_statistics_clear(void)
{
	_Ether_Stats.objects_processed = 0;
	_Ether_Stats.objects_not_invisible = 0;
	_Ether_Stats.objects_not_behind = 0;
	_Ether_Stats.objects_not_leftright = 0;
	_Ether_Stats.objects_not_abovebelow = 0;
	_Ether_Stats.objects_not_toosmall = 0;
	_Ether_Stats.lights_processed = 0;
	_Ether_Stats.facets_processed = 0;
	_Ether_Stats.facets_not_backfacing = 0;
	_Ether_Stats.facets_flatshaded = 0;
	_Ether_Stats.vertices_zchecked = 0;
	_Ether_Stats.vertices_ztransformed = 0;
	_Ether_Stats.facets_not_behind = 0;
	_Ether_Stats.vertices_xychecked = 0;
	_Ether_Stats.vertices_xytransformed = 0;
	_Ether_Stats.vertices_projected_regular = 0;
	_Ether_Stats.facets_gouraud_shaded = 0;
	_Ether_Stats.vertex_pointlights = 0;
	_Ether_Stats.vertex_dirlights = 0;
	_Ether_Stats.detail_facets = 0;
	_Ether_Stats.not_toomanyfacets = 0;
	_Ether_Stats.facets_need_x_clipping = 0;
	_Ether_Stats.facets_need_y_clipping = 0;
	_Ether_Stats.facets_need_z_clipping = 0;
}
