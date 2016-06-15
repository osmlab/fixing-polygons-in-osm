
function init_stat(data, key) {
    var radius = 1.5,
        w = 450,
        h = 400,
        margin = { top: 20, right: 50, bottom: 60, left: 100 };

    data.forEach(function(d) {
        d[0] = new Date(d[0]);
    });

/*    var t0 = data[0][0];
        t1 = data[data.length - 1][0];*/

    var t0 = new Date('2016-05-30');
        t1 = new Date('2016-09-01');

    var max = d3.max(data, function(d) {
        return d[1][key];
    });

    var scale_x = d3.time.scale()
                    .domain([t0, t1])
                    .range([0, w]);

    var axis_x = d3.svg.axis()
                    .scale(scale_x)
                    .orient('bottom')
                    .ticks(4)
                    .tickFormat(d3.time.format('%b %Y'));

    var scale_y = d3.scale.linear()
                    .domain([0, max])
                    .range([h, 0]);

    var axis_y = d3.svg.axis()
                    .scale(scale_y)
                    .orient('left');

    var chart = d3.select('#canvas_' + key).append('svg')
                    .attr('width', w + margin.left + margin.right)
                    .attr('height', h + margin.top + margin.bottom)
                    .append('g')
                        .attr('transform', 'translate(' + margin.left + ', ' + margin.top + ')')
                        .call(function(c) {
                            c.append('rect')
                                .attr('width', w + 10)
                                .attr('height', h + 10)
                                .attr('x', -5)
                                .attr('y', -5)
                                .style('fill', 'white')
                                .style('stroke', '#d0d0c8')
                        });

    chart.append('g')
        .attr('class', 'x axis')
        .attr('transform', 'translate(0, ' + (h + 5) + ')')
        .call(axis_x);

    chart.append('g')
        .attr('class', 'y axis')
        .attr('transform', 'translate(-5, 0)')
        .call(axis_y);

    chart.selectAll('circle')
        .data(data)
        .enter()
        .append('circle')
            .style('fill', '#083e76')
            .attr('cx', function(d, i) { return scale_x(d[0]); })
            .attr('cy', function(d) { return scale_y(d[1][key]); })
            .attr('r', radius)
            .attr('title', function(d, i) { return d3.time.format('%Y-%m-%d')(d[0]) + ': ' + d[1][key]; });

}

function init(error, data) {
    init_stat(data, 'from_relations')
    init_stat(data, 'from_ways')
    init_stat(data, 'nodes')
    init_stat(data, 'no_tags_on_relation')
    init_stat(data, 'intersections')
    init_stat(data, 'duplicate_nodes')
    init_stat(data, 'duplicate_segments')
    init_stat(data, 'open_rings')
    init_stat(data, 'inner_with_same_tags')
    init_stat(data, 'wrong_role')
}

d3.json('/download/area-stats.json', init);

