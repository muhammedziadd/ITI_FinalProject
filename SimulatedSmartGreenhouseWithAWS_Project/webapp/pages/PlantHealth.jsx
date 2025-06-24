import { useState } from 'react'
import axios from 'axios'

const PlantHealth = () => {
  const [selectedImage, setSelectedImage] = useState(null)
  const [selectedFile, setSelectedFile] = useState(null)
  const [analysis, setAnalysis] = useState(null)
  const [loading, setLoading] = useState(false)
  const [showReport, setShowReport] = useState(false)

  const handleImageUpload = (event) => {
    const file = event.target.files[0]
    if (file) {
      setSelectedImage(URL.createObjectURL(file))
      setSelectedFile(file)
      // Reset previous analysis and report
      setAnalysis(null)
      setShowReport(false)
    }
  }

  const handleAnalysis = async () => {
    if (!selectedFile) {
      alert('Please select an image first')
      return
    }

    const formData = new FormData()
    formData.append('image', selectedFile)
    
    setLoading(true)
    try {
      const response = await axios.post('https://xb1hsjucfa.execute-api.us-east-1.amazonaws.com/Prod/detect', formData, {
        headers: {
          'Content-Type': 'multipart/form-data'
        }
      })
      setAnalysis(response.data)
      setShowReport(true)
    } catch (error) {
      console.error('Error analyzing image:', error)
      alert('Error analyzing image. Please try again.')
    } finally {
      setLoading(false)
    }
  }

  const generatePDFReport = () => {
    // Create a new window with the report content for printing/saving as PDF
    const reportWindow = window.open('', '_blank')
    const reportContent = `
      <!DOCTYPE html>
      <html>
      <head>
        <title>Plant Health Analysis Report</title>
        <style>
          body { font-family: Arial, sans-serif; margin: 40px; line-height: 1.6; }
          .header { text-align: center; margin-bottom: 30px; border-bottom: 2px solid #30825C; padding-bottom: 20px; }
          .logo { color: #30825C; font-size: 24px; font-weight: bold; }
          .report-title { color: #333; margin: 10px 0; }
          .section { margin: 20px 0; }
          .section-title { color: #30825C; font-weight: bold; margin-bottom: 10px; border-left: 4px solid #30825C; padding-left: 10px; }
          .detail-row { margin: 8px 0; }
          .label { font-weight: bold; color: #333; }
          .value { color: #666; }
          .image-container { text-align: center; margin: 20px 0; }
          .plant-image { max-width: 400px; border: 2px solid #ddd; border-radius: 8px; }
          .recommendations { background-color: #f8f9fa; padding: 15px; border-radius: 8px; border-left: 4px solid #30825C; }
          .footer { margin-top: 40px; text-align: center; font-size: 12px; color: #666; border-top: 1px solid #ddd; padding-top: 20px; }
          .status-healthy { color: #28a745; font-weight: bold; }
          .status-unhealthy { color: #dc3545; font-weight: bold; }
          .status-warning { color: #ffc107; font-weight: bold; }
        </style>
      </head>
      <body>
        <div class="header">
          <div class="logo">🌱 Greenhouse Management System</div>
          <h1 class="report-title">Plant Health Analysis Report</h1>
          <p>Generated on: ${new Date().toLocaleDateString()} at ${new Date().toLocaleTimeString()}</p>
        </div>

        <div class="image-container">
          <img src="${selectedImage}" alt="Analyzed plant" class="plant-image" />
          <p><em>Plant Image Analyzed</em></p>
        </div>

        <div class="section">
          <h2 class="section-title">Analysis Summary</h2>
          <div class="detail-row">
            <span class="label">Health Status:</span> 
            <span class="value status-${analysis.status?.toLowerCase().includes('healthy') ? 'healthy' : analysis.status?.toLowerCase().includes('warning') ? 'warning' : 'unhealthy'}">${analysis.status || 'Unknown'}</span>
          </div>
          <div class="detail-row">
            <span class="label">Analysis Date:</span> 
            <span class="value">${new Date().toLocaleDateString()}</span>
          </div>
          <div class="detail-row">
            <span class="label">Analysis Time:</span> 
            <span class="value">${new Date().toLocaleTimeString()}</span>
          </div>
        </div>

        <div class="section">
          <h2 class="section-title">Disease Detection</h2>
          <div class="detail-row">
            <span class="label">Detected Issues:</span> 
            <span class="value">${analysis.disease || 'No diseases detected'}</span>
          </div>
          ${analysis.confidence ? `
          <div class="detail-row">
            <span class="label">Confidence Level:</span> 
            <span class="value">${analysis.confidence}%</span>
          </div>
          ` : ''}
        </div>

        <div class="section">
          <h2 class="section-title">Detailed Findings</h2>
          ${analysis.symptoms ? `
          <div class="detail-row">
            <span class="label">Observed Symptoms:</span> 
            <span class="value">${analysis.symptoms}</span>
          </div>
          ` : ''}
          ${analysis.severity ? `
          <div class="detail-row">
            <span class="label">Severity Level:</span> 
            <span class="value">${analysis.severity}</span>
          </div>
          ` : ''}
          ${analysis.affected_area ? `
          <div class="detail-row">
            <span class="label">Affected Area:</span> 
            <span class="value">${analysis.affected_area}</span>
          </div>
          ` : ''}
        </div>

        <div class="section">
          <h2 class="section-title">Recommendations</h2>
          <div class="recommendations">
            <p>${analysis.recommendations || 'No specific recommendations available.'}</p>
            ${analysis.treatment ? `
            <div style="margin-top: 15px;">
              <strong>Suggested Treatment:</strong>
              <p>${analysis.treatment}</p>
            </div>
            ` : ''}
            ${analysis.prevention ? `
            <div style="margin-top: 15px;">
              <strong>Prevention Measures:</strong>
              <p>${analysis.prevention}</p>
            </div>
            ` : ''}
          </div>
        </div>

        <div class="footer">
          <p>This report was generated by the Greenhouse Management System's AI-powered plant health analysis.</p>
          <p>For questions or concerns, please consult with a plant pathologist or agricultural expert.</p>
        </div>
      </body>
      </html>
    `
    
    reportWindow.document.write(reportContent)
    reportWindow.document.close()
    
    // Auto-trigger print dialog for PDF saving
    setTimeout(() => {
      reportWindow.print()
    }, 500)
  }

  return (
    <div className="bg-white p-6 rounded-lg shadow-lg">
      <h2 className="text-3xl font-bold mb-6 text-[#30825C]">Plant Health Analysis</h2>
      
      {/* Image Upload Section */}
      <div className="mb-6">
        <label className="block text-sm font-medium text-gray-700 mb-2">
          Upload Plant Image
        </label>
        <input
          type="file"
          accept="image/*"
          onChange={handleImageUpload}
          className="block w-full text-sm text-gray-500 file:mr-4 file:py-2 file:px-4 file:rounded-full file:border-0 file:text-sm file:font-semibold file:bg-green-50 file:text-green-700 hover:file:bg-green-100"
        />
      </div>
      
      {/* Selected Image Preview */}
      {selectedImage && (
        <div className="mb-6">
          <img 
            src={selectedImage} 
            alt="Selected plant" 
            className="max-w-md mx-auto rounded-lg border-2 border-gray-200 shadow-md" 
          />
          
          {/* Analysis Button */}
          <div className="text-center mt-4">
            <button
              onClick={handleAnalysis}
              disabled={loading}
              className="px-6 py-3 bg-[#30825C] text-white font-semibold rounded-lg hover:bg-[#145B38] disabled:opacity-50 disabled:cursor-not-allowed transition-colors"
            >
              {loading ? 'Analyzing...' : 'Start Analysis'}
            </button>
          </div>
        </div>
      )}

      {/* Loading Indicator */}
      {loading && (
        <div className="text-center py-8">
          <div className="animate-spin rounded-full h-12 w-12 border-b-2 border-[#30825C] mx-auto"></div>
          <p className="mt-4 text-gray-600">Analyzing plant health...</p>
        </div>
      )}

      {/* Analysis Results */}
      {analysis && !loading && (
        <div className="mt-6 p-6 bg-gray-50 rounded-lg border">
          <h3 className="text-2xl font-semibold mb-4 text-[#30825C]">Analysis Results</h3>
          <div className="grid grid-cols-1 md:grid-cols-2 gap-4">
            <div className="space-y-3">
              <div className="flex justify-between">
                <span className="font-medium text-gray-700">Health Status:</span>
                <span className={`font-semibold ${
                  analysis.status?.toLowerCase().includes('healthy') ? 'text-green-600' : 
                  analysis.status?.toLowerCase().includes('warning') ? 'text-yellow-600' : 'text-red-600'
                }`}>
                  {analysis.status}
                </span>
              </div>
              <div className="flex justify-between">
                <span className="font-medium text-gray-700">Disease Detection:</span>
                <span className="text-gray-600">{analysis.disease || 'No diseases detected'}</span>
              </div>
              {analysis.confidence && (
                <div className="flex justify-between">
                  <span className="font-medium text-gray-700">Confidence:</span>
                  <span className="text-gray-600">{analysis.confidence}%</span>
                </div>
              )}
            </div>
            <div>
              <span className="font-medium text-gray-700">Recommendations:</span>
              <p className="text-gray-600 mt-1">{analysis.recommendations}</p>
            </div>
          </div>
        </div>
      )}

      {/* Report Generation Section */}
      {showReport && analysis && (
        <div className="mt-8 p-6 bg-blue-50 rounded-lg border-2 border-blue-200">
          <h3 className="text-2xl font-semibold mb-4 text-blue-800 flex items-center">
            📊 Generate Report
          </h3>
          <p className="text-blue-700 mb-4">
            Create a comprehensive PDF report of this plant health analysis including the image, 
            detailed findings, and recommendations.
          </p>
          <div className="flex flex-col sm:flex-row gap-4">
            <button
              onClick={generatePDFReport}
              className="flex-1 px-6 py-3 bg-blue-600 text-white font-semibold rounded-lg hover:bg-blue-700 transition-colors flex items-center justify-center"
            >
              📄 Download PDF Report
            </button>
            <button
              onClick={() => setShowReport(false)}
              className="flex-1 px-6 py-3 bg-gray-500 text-white font-semibold rounded-lg hover:bg-gray-600 transition-colors"
            >
              Close Report Section
            </button>
          </div>
          
          {/* Report Preview */}
          <div className="mt-6 p-4 bg-white rounded border">
            <h4 className="font-semibold text-gray-800 mb-2">Report will include:</h4>
            <ul className="text-gray-600 space-y-1">
              <li>• Plant image and analysis timestamp</li>
              <li>• Health status and disease detection results</li>
              <li>• Detailed findings and symptoms</li>
              <li>• Treatment recommendations</li>
              <li>• Professional formatting for documentation</li>
            </ul>
          </div>
        </div>
      )}
    </div>
  )
}

export default PlantHealth