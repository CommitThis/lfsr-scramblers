import os
import csv
import platform
import re
import argparse
import magic

from collections import defaultdict

import plotly.graph_objects as go


script_dir = os.path.dirname(__file__)

def get_encoding(csv_file):
    fd = open(csv_file, 'rb')
    if not fd:
        raise Exception("Error opening file")
    mime = magic.Magic(mime_encoding=True)
    encoding = mime.from_buffer(fd.read())
    if encoding == 'utf-16le':
        encoding = 'utf-16' # ???? why ????
    fd.close()
    return encoding


def get_results(csv_file):
    encoding = get_encoding(csv_file)

    fd = open(csv_file, 'r', encoding=encoding)
    if not fd:
        raise Exception("Error opening file")

    results = defaultdict(list)

    reader = csv.DictReader(fd)
    fieldnames = [field.strip() for field in reader.fieldnames]

    for record in reader:
        print(record)
        name_regex = r'LFSR_([A-Za-z]+)<Degree_(\d+)>.*'
        match = re.match(name_regex, record['name'])
        real_time = float(record['real_time'])
        cpu_time = float(record['cpu_time'])
        if match:
            suite = match.group(1)
            degree = int(match.group(2))
            results[suite].append((
                    degree, 
                    real_time, 
                    cpu_time)
                )
    return results




if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Plot benchmark results')
    parser.add_argument('--input-file', type=str, help='CSV file containing benchmark results')
    parser.add_argument('--write-image', action='store_true', help='Output file for the plot')
    parser.add_argument('--platform', type=str, required=True, help='Platform name')
    args = parser.parse_args()

    results = get_results(args.input_file)
    
    fib_results      = results['FeedthroughFibonacci']
    fib_bulk_results = results['FeedthroughFibonacciBulk']
    galois_results   = results['FeedthroughGalois']

    x_f  = [ a[0] for a in fib_results ]
    y_f  = [ a[1] for a in fib_results ] # real time
    x_bf = [ a[0] for a in fib_bulk_results ]
    y_bf = [ a[1] for a in fib_bulk_results ] # real time
    x_g  = [ a[0] for a in galois_results ]
    y_g  = [ a[1] for a in galois_results ] # real time

    min_x = min(x_f)
    max_x = max(x_f)
    max_y = max(max(y_f), max(y_bf), max(y_g))

    fig = go.Figure()
    fig.add_trace(go.Scatter(x=x_f,  y=y_f,  
            mode='lines+markers', 
            name='Fibonacci', 
            line=dict(color='#f147ab', width=2), 
            marker=dict(color='#f147ab', size=4),))
    fig.add_trace(go.Scatter(x=x_bf, y=y_bf, 
            mode='lines+markers', 
            name='FibonacciBulk', 
            line=dict(color='#B511F2', width=2), 
            marker=dict(color='#B511F2', size=4),))
    fig.add_trace(go.Scatter(x=x_g,  y=y_g,  
            mode='lines+markers', 
            name='Galois', 
            line=dict(color='#559CFF', width=2), 
            marker=dict(color='#559CFF', size=4),))
    fig.update_layout(
        title=f'Wall Time per Degree<br><sup>Average time per run, 4-tap LFSR ({args.platform})</sup>',
        paper_bgcolor='#151515',
        xaxis_title='Degree',
        yaxis_title='Time (ns)',
        template='plotly_dark',
        legend={
                'x': 0.02,
                'y': 0.98,
                'xanchor': 'left',
                'bgcolor': 'rgba(51,51,51, 0.5)'
            },
        margin={
            'l': 20, 
            'r': 20, 
            't': 80, 
            'b': 20
        },
        width=800,
        height=500)

    fig.update_xaxes(showgrid=True, gridwidth=1, gridcolor='#222', range=[min_x, max_x], nticks=(max_x - min_x + 1))
    fig.update_yaxes(showgrid=True, gridwidth=1, gridcolor='#222', range=[0, max_y + max_y * 0.05], nticks=20)
    if args.write_image:
        fig.write_image(os.path.join(os.getcwd(), f'{args.platform}.png'))
    fig.show()



