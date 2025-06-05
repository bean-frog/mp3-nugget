import { useState, useEffect } from 'react';
import "./style.css"
export default function App() {
  const [gridSize, setGridSize] = useState(16);
  const [grid, setGrid] = useState([]);
  const [showExport, setShowExport] = useState(false);
  const [exportedCode, setExportedCode] = useState('');
  const [showImport, setShowImport] = useState(false);
  const [importCode, setImportCode] = useState('');

  useEffect(() => {
    const newGrid = Array(gridSize).fill().map(() => Array(gridSize).fill(0));
    setGrid(newGrid);
  }, [gridSize]);
  
    const importBitmap = (cppCode) => {
      try {
        const sizeMatch = cppCode.match(/const uint8_t iconBitmap\[(\d+)\]\[(\d+)\]/);
        
        if (!sizeMatch || sizeMatch.length < 3) {
          alert("Format issue, cant determine size");
        }
        
        const size = parseInt(sizeMatch[1], 10);
        
        if (size < 8 || size > 64) {
          alert(`Grid size ${size} is out of allowed range (8-64).`);
        }
        
        const valueMatch = cppCode.match(/\{([\s\S]*?)\};/);
        if (!valueMatch) {
          alert("Cannot find array values.");
        }
        
        const rowsStr = valueMatch[1].trim().split('\n').map(row => row.trim());
        const newGrid = [];
        
        for (let i = 0; i < rowsStr.length; i++) {
          let row = rowsStr[i].replace(/[{}]/g, '').trim();
          if (row.endsWith(',')) {
            row = row.slice(0, -1);
          }
          
          const values = row.split(',').map(val => parseInt(val.trim(), 10));
          
          if (values.length !== size) {
            alert(`Row ${i + 1} has ${values.length} values, expected ${size}.`);
          }
          
          newGrid.push(values);
        }
        
        if (newGrid.length !== size) {
          alert(`Found ${newGrid.length} rows, expected ${size}.`);
        }
        
        setGridSize(size);
        setGrid(newGrid);
        return true;
      } catch (error) {
        console.error("Import failed:", error.message);
        alert("Import failed, check console");
        return false;
      }
    };
    


  const handleSizeChange = (e) => {
    const size = parseInt(e.target.value, 10);
    if (size >= 8 && size <= 64) {
      setGridSize(size);
    }
  };

  const toggleCell = (row, col) => {
    const newGrid = [...grid];
    newGrid[row][col] = newGrid[row][col] === 0 ? 1 : 0;
    setGrid(newGrid);
  };

  const exportToCpp = () => {
    let code = `const uint8_t iconBitmap[${gridSize}][${gridSize}] = {\n`;
    
    grid.forEach((row, rowIndex) => {
      code += '  {';
      row.forEach((cell, colIndex) => {
        code += cell;
        if (colIndex < row.length - 1) {
          code += ', ';
        }
      });
      code += '}';
      if (rowIndex < grid.length - 1) {
        code += ',';
      }
      code += '\n';
    });
    
    code += '};';
    
    setExportedCode(code);
    setShowExport(true);
  };

  const copyToClipboard = () => {
    navigator.clipboard.writeText(exportedCode)
      .then(() => {
        alert('copied!');
      })
      .catch(err => {
        alert("Couldn't copy, check console")
        console.error('Failed to copy: ', err);
      });
  };
  
  const handleImport = () => {
    importBitmap(importCode);
    setShowImport(false);
    setImportCode('');
  };

  return (
    <div className="flex flex-col items-center p-4 mx-auto max-w-4xl">
      <h1 className="mb-4 text-2xl font-bold">Bitmap Grid Editor</h1>
      
      <div className="mb-6 w-full max-w-md">
        <label className="block mb-2 font-medium">
          Grid Size (8-64):
          <input
            type="number"
            min="8"
            max="64"
            value={gridSize}
            onChange={handleSizeChange}
            className="p-1 ml-2 rounded border"
          />
        </label>
      </div>
      
      <div 
        className="grid mb-4 bg-gray-100 border border-gray-300"
        style={{
          gridTemplateColumns: `repeat(${gridSize}, minmax(0, 1fr))`,
          gap: '1px',
          width: '100%',
          maxWidth: '500px',
          aspectRatio: '1/1'
        }}
      >
        {grid.map((row, rowIndex) => (
          row.map((cell, colIndex) => (
            <div
              key={`${rowIndex}-${colIndex}`}
              className={`cursor-pointer ${cell === 0 ? 'bg-black' : 'bg-white'}`}
              onClick={() => toggleCell(rowIndex, colIndex)}
              style={{
                aspectRatio: '1/1'
              }}
            />
          ))
        ))}
      </div>
      
      <div className="flex space-x-4">
        <button
          onClick={exportToCpp}
          className="px-4 py-2 text-white bg-blue-500 rounded hover:bg-blue-600"
        >
          Export as C++ Array
        </button>
        <button
          onClick={() => setShowImport(true)}
          className="px-4 py-2 text-white bg-green-500 rounded hover:bg-green-600"
        >
          Import C++ Array
        </button>
      </div>
      
      {showExport && (
        <div className="flex fixed inset-0 justify-center items-center p-4 bg-black bg-opacity-50">
          <div className="p-6 w-full max-w-2xl bg-white rounded-lg">
            <h2 className="mb-4 text-xl font-bold">C++ 2D Array</h2>
            <pre className="overflow-auto p-4 max-h-96 text-sm bg-gray-100 rounded">
              {exportedCode}
            </pre>
            <div className="flex justify-end mt-4 space-x-4">
              <button
                onClick={copyToClipboard}
                className="px-4 py-2 text-white bg-green-500 rounded hover:bg-green-600"
              >
                Copy to Clipboard
              </button>
              <button
                onClick={() => setShowExport(false)}
                className="px-4 py-2 text-white bg-gray-500 rounded hover:bg-gray-600"
              >
                Close
              </button>
            </div>
          </div>
        </div>
      )}
      
      {showImport && (
        <div className="flex fixed inset-0 justify-center items-center p-4 bg-black bg-opacity-50">
          <div className="p-6 w-full max-w-2xl bg-white rounded-lg">
            <h2 className="mb-4 text-xl font-bold">Import 2D Array</h2>
            <p className="mb-2 text-sm text-gray-600">paste here:</p>
            <textarea
              value={importCode}
              onChange={(e) => setImportCode(e.target.value)}
              className="p-2 w-full h-64 font-mono text-sm rounded border"
              placeholder="const uint8_t iconBitmap[16][16] = {&#10;  {1, 1, 1, ...},&#10;  ...&#10;};"
            />
            <div className="flex justify-end mt-4 space-x-4">
              <button
                onClick={handleImport}
                className="px-4 py-2 text-white bg-green-500 rounded hover:bg-green-600"
              >
                Import
              </button>
              <button
                onClick={() => setShowImport(false)}
                className="px-4 py-2 text-white bg-gray-500 rounded hover:bg-gray-600"
              >
                Cancel
              </button>
            </div>
          </div>
        </div>
      )}
    </div>
  );
}