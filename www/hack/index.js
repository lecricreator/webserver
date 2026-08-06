const fileInput = document.getElementById("fileUpload")
const imageOutput = document.getElementById("output");

fileInput.addEventListener("change", async () => {
    let [file] = fileInput.files

    const reader = new FileReader();
    reader.onload = (e) => {
        imageOutput.src = e.target.result;
    };

    reader.onerror = (err) => {
        console.error("Error reading file:", err);
        alert("An error occurred while reading the file.");
    };


    reader.readAsDataURL(file);
})
