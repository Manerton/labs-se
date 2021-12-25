import express = require('express');
import path = require('path');

const REACT_BUILD_PATH = path.join(__dirname, "../../chat-app/build");

/** Класс - веб-сервер Express. */
export class ExpressApp
{
    /** Приложение Express. */
    public app: express.Express = express();

    constructor()
    {
        this.app.use(ExpressApp.wwwMiddleware);

        this.app.use(ExpressApp.httpsMiddleware);

        this.app.disable('x-powered-by');

        this.app.use(ExpressApp.rejectRequestWithBodyMiddleware);

        this.handleStatic();

        this.handleRoutes();

        this.app.use(ExpressApp.preventFloodMiddleware);

        this.endPoint();
    }

    /** Убираем www из адреса. */
    private static wwwMiddleware(req: express.Request, res: express.Response, next: express.NextFunction): void
    {
        if (req.hostname?.slice(0, 4) === 'www.')
        {
            const newHost: string = req.hostname.slice(4);
            res.redirect(301, req.protocol + '://' + newHost + req.originalUrl);
        }
        else
        {
            next();
        }
    }
    /** Перенаправляем на https. */
    private static httpsMiddleware(req: express.Request, res: express.Response, next: express.NextFunction): void
    {
        if (!req.secure)
        {
            res.redirect(301, ['https://', req.hostname, req.originalUrl].join(''));
        }
        else
        {
            next();
        }
    }

    /** Есть ли тело у запроса? */
    public static requestHasNotBody(req: express.Request): boolean
    {
        const contentLength = req.headers["content-length"];
        return ((!contentLength || Number(contentLength) == 0) && req.readableLength == 0);
    }

    /** Отправить код ошибки и разорвать соединение с клиентом. */
    public static sendCodeAndDestroySocket(req: express.Request, res: express.Response, httpCode: number): void
    {
        // экспериментальным путем установлено, что чтение 13 чанков по 16 кб (208 кб) и последующее уничтожение сокета реквеста
        // с большой вероятностью возвращает код http респонса для Chrome и Postman,
        // причем Postman сам отправляет 13 чанков и после сразу принимает код реквеста (судя по логу, если отслеживать event 'data' у реквеста),
        // а Chrome, если ничего не делать, отправляет данные пока не кончится файл и только потом отображает код респонса
        // Firefox тоже отправляет данные пока не кончится файл и только потом отображает код респонса
        // однако 13 чанков и последующий destroy реквеста ему не особо помогают, он просто у себя фиксирует, что реквест оборвался, но без кода ответа
        let i = 0;

        req.on("data", () =>
        {
            if (i++ == 12) req.socket.destroy();
        });

        res.status(httpCode).end();
    }

    /** Отвергаем запросы GET, HEAD и OPTIONS с телом в запросе. */
    private static rejectRequestWithBodyMiddleware(req: express.Request, res: express.Response, next: express.NextFunction): void
    {
        const methodWithoutBody = (req.method == "GET" || req.method == "HEAD" || req.method == "OPTIONS");
        if (methodWithoutBody && !ExpressApp.requestHasNotBody(req))
        {
            ExpressApp.sendCodeAndDestroySocket(req, res, 405);
        }
        else
        {
            next();
        }
    }

    /** Защищаемся от флуд-атаки через body в реквесте. */
    private static preventFloodMiddleware(req: express.Request, res: express.Response, next: express.NextFunction): void
    {
        if (ExpressApp.requestHasNotBody(req))
        {
            next();
        }
        else
        {
            ExpressApp.sendCodeAndDestroySocket(req, res, 405);
        }
    }

    /** Обрабатываем статику. */
    private handleStatic(): void
    {
        this.app.use(express.static(path.join(REACT_BUILD_PATH)));
    }

    /** Обрабатываем маршруты. */
    private handleRoutes(): void
    {
        this.app.get('/*', (req: express.Request, res: express.Response) =>
        {
            res.sendFile(path.join(REACT_BUILD_PATH, "index.html"));
        });
    }

    /** Самый последний обработчик запросов. */
    private endPoint(): void
    {
        this.app.use((req: express.Request, res: express.Response) =>
        {
            let statusCode = 405;
            if (req.method == "GET" || req.method == "HEAD")
            {
                statusCode = 404;
            }
            res.sendStatus(statusCode);
        });
    }
}
