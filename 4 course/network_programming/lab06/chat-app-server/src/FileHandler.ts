import express = require("express");
import path = require("path");
import { nanoid } from "nanoid";
import fs = require('fs');

type FileId = string;

interface FileInfo
{
    name: string;
}

// класс - обработчик файлов
export class FileHandler
{
    private readonly FILES_PATH = path.join(process.cwd(), "/files");
    private fileStorage = new Map<FileId, FileInfo>();

    public getFileInfo(fileId: string): FileInfo | undefined
    {
        return this.fileStorage.get(fileId);
    }
    public handleFileDownload(
        req: express.Request,
        res: express.Response
    ): void
    {
        const fileId: FileId = req.params.fileId;
        const fileInfo = this.getFileInfo(fileId);

        if (!fileInfo)
        {
            return res.status(404).end();
        }

        return res.download(path.join(this.FILES_PATH, fileId), fileInfo.name ?? fileId);
    }
    public handleFileUpload(
        req: express.Request,
        res: express.Response,
    ): void
    {
        const fileId: string = nanoid(32);

        // проверяем, существует ли папка для файлов
        if (!fs.existsSync(this.FILES_PATH))
            fs.mkdirSync(this.FILES_PATH);

        // указываем путь и название
        const filePath = path.join(this.FILES_PATH, fileId);
        const outStream = fs.createWriteStream(filePath);

        let filename = req.header("Upload-Filename")!;

        filename = Buffer.from(filename, "base64").toString("utf-8");

        this.fileStorage.set(fileId, { name: filename });

        outStream.on("finish", () =>
        {
            return res.status(201).end(fileId);
        });

        req.pipe(outStream);
    }
}