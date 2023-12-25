from flask import Flask, jsonify, request
from flask_cors import CORS
import subprocess
import os

app = Flask(__name__)
CORS(app)

current_dir = os.path.dirname(os.path.abspath(__file__))

@app.route('/process_text', methods=['POST'])
def process_text():
    input_text = request.json.get('inputText')
    
    cnf_file_path = os.path.join(current_dir, 'temp.cnf')
    with open('temp.cnf', 'w') as cnf_file:
        cnf_file.write(input_text)

    solver_executable = os.path.join(current_dir, '..', 'cpp', 'build', 'LTLSolver')

    command = [solver_executable, cnf_file_path]
    result = subprocess.run(command, capture_output=True, text=True)

    if result.returncode == 0 and result.stdout.strip() == "SAT":
        output_text = "Satisfiable"
    else:
        output_text = "Unsatisfiable"

    return jsonify({'outputText': output_text})

if __name__ == '__main__':
    app.run(debug=True)