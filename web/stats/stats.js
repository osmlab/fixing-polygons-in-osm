
function init_stat(data, key) {
    var radius = 1.5,
        w = 1000,
        h = 400,
        margin = { top: 20, right: 20, bottom: 20, left: 100 };

    var t0 = data[0][0];
        t1 = data[data.length - 1][0];

/*    var t0 = new Date('2016-05-30');
        t1 = new Date('2017-01-01');*/

    var max = d3.max(data, function(d) {
        return d[1][key];
    });

    var scale_x = d3.time.scale()
                    .domain([t0, t1])
                    .range([0, w]);

    var axis_x = d3.svg.axis()
                    .scale(scale_x)
                    .tickSize(-(h + 10))
                    .orient('bottom');

    var scale_y = d3.scale.linear()
                    .domain([0, max])
                    .range([h, 0]);

    var axis_y = d3.svg.axis()
                    .scale(scale_y)
                    .tickSize(-(w + 10))
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
                                .style('fill', '#f0f0f0')
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

    var line = d3.svg.line()
        .interpolate("linear")
        .x(function(d) { return scale_x(d[0]); })
        .y(function(d) { return scale_y(d[1][key]); });

    chart.selectAll('.line')
        .data(["foo"])
        .enter()
        .append('path')
            .datum(data)
            .attr("class", "line")
            .attr("d", line);

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

function init_old_style_percent(data) {
    var radius = 1.5,
        w = 1000,
        h = 200,
        margin = { top: 20, right: 20, bottom: 20, left: 100 };

    var t0 = data[0][0];
        t1 = data[data.length - 1][0];

/*    var t0 = new Date('2016-05-30');
        t1 = new Date('2017-01-01');*/

    var max = 12;

    var scale_x = d3.time.scale()
                    .domain([t0, t1])
                    .range([0, w]);

    var axis_x = d3.svg.axis()
                    .scale(scale_x)
                    .tickSize(-(h + 10))
                    .orient('bottom');

    var scale_y = d3.scale.linear()
                    .domain([0, max])
                    .range([h, 0]);

    var axis_y = d3.svg.axis()
                    .scale(scale_y)
                    .ticks(8)
                    .tickSize(-(w + 10))
                    .tickFormat(function(x) { return '' + x + '%';})
                    .orient('left');

    var chart = d3.select('#canvas_old_style_percent').append('svg')
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
                                .style('fill', '#f0f0f0')
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

    var value = function(d) {
        return d[1]['no_tags_on_relation'] * 100.0 / d[1]['from_relations'];
    }

    var line = d3.svg.line()
        .interpolate("linear")
        .x(function(d) { return scale_x(d[0]); })
        .y(function(d) { return scale_y(value(d)); });

    chart.selectAll('.line')
        .data(["foo"])
        .enter()
        .append('path')
            .datum(data)
            .attr("class", "line")
            .attr("d", line);

    chart.selectAll('circle')
        .data(data)
        .enter()
        .append('circle')
            .style('fill', '#083e76')
            .attr('cx', function(d, i) { return scale_x(d[0]); })
            .attr('cy', function(d) { return scale_y(value(d)); })
            .attr('r', radius)
            .attr('title', function(d, i) { return d3.time.format('%Y-%m-%d')(d[0]) + ': ' + (Math.round(value(d) * 10)/10) + '%'; });
}

function init(error, data) {
    data.forEach(function(d) {
        d[0] = new Date(d[0]);
    });

    init_stat(data, 'from_relations');
    init_stat(data, 'from_ways');
    init_stat(data, 'nodes');
    init_stat(data, 'no_tags_on_relation');
    init_stat(data, 'intersections');
    init_stat(data, 'duplicate_nodes');
    init_stat(data, 'duplicate_segments');
    init_stat(data, 'open_rings');
    init_stat(data, 'inner_with_same_tags');
    init_stat(data, 'wrong_role');

    init_old_style_percent(data);
}

