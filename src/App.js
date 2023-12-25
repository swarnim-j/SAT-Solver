import React, { useState } from 'react';

function App() {
  const [inputText, setInputText] = useState('');
  const [displayedText, setDisplayedText] = useState('');

  const handleButtonClick = async () => {
    try {
      const response = await fetch('http://127.0.0.1:5000/process_text', {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify({ inputText }),
      });

      if (!response.ok) {
        throw new Error('Network response was not ok');
      }

      const data = await response.json();
      setDisplayedText(data.outputText);
    } catch (error) {
      console.error('Error:', error);
    }
  };

  return (
    <div>
      <div>
        <label>
          Enter CNF formula (DIMACS): 
          <textarea
            value={inputText}
            onChange={(e) => setInputText(e.target.value)}
          />
        </label>
        <button onClick={handleButtonClick}>Solve</button>
      </div>
      <div>
        <p>SAT/UNSAT: {displayedText}</p>
      </div>
    </div>
  );
}

export default App;
