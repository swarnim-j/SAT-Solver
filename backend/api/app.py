# app.py

from flask import Flask, jsonify
import subprocess

app = Flask(__name__)

def run_solver():
    try:
        solver_path = 'build/LTLSolver'
        result = subprocess.run([solver_path], check=True, capture_output=True, text=True)
        return result.stdout
    except subprocess.CalledProcessError as e:
        return f"Error: {e}"
    except Exception as e:
        return f"Unexpected Error: {e}"

@app.route('/api/data', methods=['GET'])
def get_data():
    
    data = {'message': run_solver()}
    return jsonify(data)

if __name__ == '__main__':
    app.run(debug=True)
