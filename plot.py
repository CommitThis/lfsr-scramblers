import os
import csv
import platform
import re
import plotly.graph_objects as go
import plotly.express as px

from collections import defaultdict

script_dir       = os.path.dirname(__file__)
csv_result_file  = os.path.join(script_dir, 'bench.csv')
csv_encoding     = 'utf-16' if platform.system() == 'Windows' else 'utf-8'

fd = open(csv_result_file, 'r', encoding=csv_encoding)
if not fd:
    print("Error opening file")
    exit(1)


results = defaultdict(list)

reader = csv.DictReader(fd)
for record in reader:
    name_regex = r'LFSR_([A-Za-z]+)<Degree_(\d+)>.*'
    match = re.match(name_regex, record['name'])
    if match:
        suite = match.group(1)
        degree = match.group(2)
        results[suite].append((degree, float(record['real_time']), float(record['cpu_time'])))


fib_results      = results['FeedthroughFibonacci']
fib_bulk_results = results['FeedthroughFibonacciBulk']
galois_results   = results['FeedthroughGalois']

x_f  = [ a[0] for a in fib_results ]
y_f  = [ a[2] for a in fib_results ] # real time
x_bf = [ a[0] for a in fib_bulk_results ]
y_bf = [ a[2] for a in fib_bulk_results ] # real time
x_g  = [ a[0] for a in galois_results ]
y_g  = [ a[2] for a in galois_results ] # real time



fig = go.Figure()
fig.add_trace(go.Scatter(x=x_f,  y=y_f,  mode='lines+markers', name='Fibonacci'))
fig.add_trace(go.Scatter(x=x_bf, y=y_bf, mode='lines+markers', name='FibonacciBulk'))
fig.add_trace(go.Scatter(x=x_g,  y=y_g,  mode='lines+markers', name='Galois'))
fig.update_layout(
    title='Wall Time per Degree',
    xaxis_title='Degree',
    yaxis_title='Time (ns)',
    )
fig.show()

